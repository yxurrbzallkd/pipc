#include <pipc/shmem/shmem.hpp>
#include <sys/wait.h>

using namespace std;

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc == 1) {
		char* program_name = argv[0];
		char * shm_name = (char*)"/shm_object";
		// IMPORTANT!!! Pass O_RDWR else HANGS
		pipc::shmem sp(shm_name, BUF_SIZE, O_RDWR, PROT_READ);
		if (sp.setup() != SUCCESS)
			return -1;
		pid_t pid = fork();
		if (pid < 0)
			return -4;
		if (pid == 0)
			execlp(program_name, program_name, shm_name, NULL);
		int res;
		waitpid(pid, &res, 0);
		if (res != SUCCESS)
			return res;
		char buf[1024];
		if (sp.read_shm(buf, 1024, 0) != SUCCESS)
			return -2;
		std::cout << buf << std::endl;
		return 0;
	} else if (argc == 2) {
		char* shm_name = argv[1];
		pipc::shmem sc(shm_name, O_WRONLY, PROT_WRITE);
		if (sc.setup() != SUCCESS)
			exit(-1);
		char hello[] = "hello world!";
		if (sc.write_shm(hello, 13, 0) != SUCCESS)
			exit(-2);
		exit(0);
	}
	return 0;
}
