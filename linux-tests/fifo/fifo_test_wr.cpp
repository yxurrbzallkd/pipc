#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	char* program_name = argv[0];
	if (argc == 1) {
		// IMPORTANT!!! Pass O_RDWR else HANGS
		pipc::fifo fp(fifo_name, true, FF_RDWR);
		if (fp.setup() != SUCCESS) return -1;

		char hello[13] = "hello world!";
		if (fp.write_fifo(hello, 13) != SUCCESS)
			return -2;
		pid_t pid = fork();
		if (pid < 0)
			return -4;
		int res;
		if (pid == 0) {
			execlp(program_name, program_name, "-", NULL);
		} else {
			waitpid(pid, &res, 0);
		}
		if (res != SUCCESS) return res;
	} else {
		fifo_name = argv[1];
		pipc::fifo fc(fifo_name, false, FF_RD);
		if (fc.setup() != SUCCESS) exit(-1);
		char buf[1024];
		if (fc.read_fifo(buf, 1024) != SUCCESS) exit(-2);
		std::cout << buf << std::endl;
		exit(0);
	}
	return 0;
}
