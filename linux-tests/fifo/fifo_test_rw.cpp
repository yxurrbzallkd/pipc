#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char programName[] = "./fifo_test_rw";
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc == 2) {
		std::cout << argc << std::endl;
		// IMPORTANT!!! Pass O_RDWR else HANGS
		pipc::fifo fp(fifo_name, true, O_RDWR);
		if (fp.setup() != SUCCESS) {
			std::cout << "setup failed" << std::endl;
			return -1;
		}
		std::cout << "execlp" << std::endl;
		int res;
		pid_t pid = fork();
		if (fork() < 0)
			return -4;
		if (pid == 0)
			execlp(programName, programName, NULL);
		else {
			waitpid(pid, &res, 0);
			std::cout << "done" << std::endl;
		}
		if (res != SUCCESS) {
			std::cout << res << std::endl;
			fp.unlink_fifo();
			return res;
		}
		char buf[1024];
		if (fp.read_fifo(buf, 1024) != SUCCESS) {
			std::cout << "read fail" << std::endl;
			fp.unlink_fifo();
			return -2;
		}
		if (fp.unlink_fifo() != SUCCESS) {
			std::cout << "unlink failed" << std::endl;
			return -3;
		}
		std::cout << "success" << std::endl;
		std::cout << buf << std::endl;
	} else {
		std::cout << "argc " << argc << std::endl;
		char hello[] = "hello world!";
		pipc::fifo fc(fifo_name, false, O_WRONLY);
		if (fc.setup() != SUCCESS) {
			std::cout << "setup failed" << std::endl;
			return -1;
		}
		if (fc.write_fifo(hello, 13) != SUCCESS) {
			std::cout << "write failed" << std::endl;
			return -2;
		}
		return 0;
	}
	return 0;
}
