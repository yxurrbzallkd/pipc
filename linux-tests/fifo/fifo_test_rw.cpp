#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char* programName = argv[0];
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
		if (fork() < 0) {
			fp.unlink_fifo();
			return -4;
		}
		if (pid == 0) {
			execlp(programName, "-", NULL);
		}
		waitpid(pid, &res, 0);
		std::cout << "check " << res << std::endl;
		if (res != SUCCESS) {
			std::cout << res << std::endl;
			fp.unlink_fifo();
			return res;
		}
		std::cout << "reading" << std::endl;
		char buf[1024];
		if (fp.read_fifo(buf, 1024) != SUCCESS) {
			std::cout << "read fail" << std::endl;
			fp.unlink_fifo();
			return -2;
		}
		std::cout << "unlinking" << std::endl;
		if (fp.unlink_fifo() != SUCCESS) {
			std::cout << "unlink failed" << std::endl;
			return -3;
		}
		std::cout << "success" << std::endl;
		std::cout << buf << std::endl;
		return 0;
	} else if (argc == 1) {
		std::cout << "argc " << argc << std::endl;
		char hello[] = "hello world!";
		pipc::fifo fc(fifo_name, false, O_WRONLY);
		if (fc.setup() != SUCCESS) {
			std::cout << "setup failed" << std::endl;
			exit(-1);
		}
		if (fc.write_fifo(hello, 13) != SUCCESS) {
			std::cout << "write failed" << std::endl;
			exit(-2);
		}
		std::cout << "write success" << std::endl;
		exit(0);
	}
	return 0;
}
