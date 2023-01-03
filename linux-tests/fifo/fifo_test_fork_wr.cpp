#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char fifo_name[] = "/tmp/fifo";
	// IMPORTANT!!! Pass O_RDWR, otherwise HANGS
	pipc::fifo fp(fifo_name, true, FF_RDWR);
	if (fp.setup() != SUCCESS) {
		std::cerr << "failed setup" << std::endl;
		return -1;
	}

	char hello[13] = "hello world!";
	if (fp.write_fifo(hello, 13) != SUCCESS) {
		std::cerr << "failed to write" << std::endl;
		return -1;
	}
	pid_t pid = fork();
	if (pid < 0) {
		std::cerr << "failed to fork" << std::endl;
		return -1;
	}
	if (pid == 0) {
		pipc::fifo fc(fifo_name, false, FF_RD);
		if (fc.setup() != SUCCESS) {
			std::cerr << "failed setup" << std::endl;
			exit(-1);
		}
		char buf[1024];
		if (fc.read_fifo(buf, 1024) != SUCCESS) {
			std::cerr << "failed to read" << std::endl;
			exit(-1);
		}
		std::cout << buf << std::endl;
		exit(0);
	}
	int status;
	waitpid(pid, &status, 0);
	if (status != SUCCESS) {
		std::cerr << "child process nonzero exit" << std::endl;
		return -1;
	}
	return 0;
}
