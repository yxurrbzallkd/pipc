#include <iostream>
#include <pipc/shmem/shmem.hpp>
#include <sys/wait.h>

using namespace std;

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc == 1) {
		char* program_name = argv[0];
		char shm_name[] = "/shm_object";
		// IMPORTANT!!! Pass O_RDWR else HANGS
		pipc::shmem sp(shm_name, BUF_SIZE, SF_WR, MF_WR);
		if (sp.setup() != SUCCESS)
			return -1;

		char hello[13] = "hello world!";
		if (sp.write_shm(hello, 13, 0) != SUCCESS)
			return -2;

		pid_t pid = fork();
		if (pid < 0)
			return -4;
		
		int res;
		if (pid == 0)
			execlp(program_name, program_name, shm_name, NULL);
		waitpid(pid, &res, 0);
		if (res != SUCCESS)
			return res;
	} else if (argc == 2) {
		char* shm_name = argv[1];
		pipc::shmem sc(shm_name, SF_RD, MF_RD);
		if (sc.setup() != SUCCESS)
			exit(-1);
		char buf[BUF_SIZE];
		if (sc.read_shm(buf, BUF_SIZE, 0) != SUCCESS)
			exit(-2);
		std::cout << buf << std::endl;
		exit(0);
	}
	return 0;
}
