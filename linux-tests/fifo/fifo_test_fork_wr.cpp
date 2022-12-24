#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char fifo_name[] = "/tmp/fifo";
	// IMPORTANT!!! Pass O_RDWR, otherwise HANGS
	pipc::fifo fp(fifo_name, true, O_RDWR);
	if (fp.setup() != SUCCESS) return -1;

	char hello[13] = "hello world!";
	if (fp.write_fifo(hello, 13) != SUCCESS) {
		fp.unlink_fifo();
		return -1;
	}
	pid_t pid = fork();
	if (pid < 0)
		return -1;
	if (pid == 0) {
		pipc::fifo fc(fifo_name, false, O_RDONLY);
		if (fc.setup() != SUCCESS) {
			std::cout << "failed setup" << std::endl;
			exit(-1);
		}
		char buf[1024];
		if (fc.read_fifo(buf, 1024) != SUCCESS) {
			std::cout << "failed read" << std::endl;
			exit(-1);
		}
		std::cout << buf << std::endl;
		exit(0);
	}
	int status;
	waitpid(pid, &status, 0);
	if (fp.unlink_fifo() != SUCCESS) return -1;
	if (status != SUCCESS) return -1;
	return 0;
}
