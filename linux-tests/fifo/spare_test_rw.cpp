#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char* program_name = (char*)"./rw_child";
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc > 1)
		fifo_name = argv[1];
	if (argc > 2)
		program_name = argv[2];
	// IMPORTANT!!! Pass O_RDWR else HANGS
	pipc::fifo fp(fifo_name, true, O_RDWR);
	if (fp.setup() != SUCCESS)
		return -1;

	int res;
	pid_t pid = fork();
	if (pid < 0)
		return -4;
	if (pid == 0)
		execlp(program_name, program_name, NULL);

	waitpid(pid, &res, 0);
	if (res != SUCCESS)
		return res;

	char buf[1024];
	if (fp.read_fifo(buf, 1024) != SUCCESS)
		return -2;

	if (fp.unlink_fifo() != SUCCESS)
		return -3;
	
	std::cout << buf << std::endl;
	return 0;
}
