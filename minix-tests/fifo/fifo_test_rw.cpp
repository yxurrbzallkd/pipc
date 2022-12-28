#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char* programName = argv[0];
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc == 2) {
		// IMPORTANT!!! Pass O_RDWR else HANGS
		pipc::fifo fp(fifo_name, true, O_RDWR);
		if (fp.setup() != SUCCESS)
			return -1;
		int res;
		pid_t pid = fork();
		if (pid < 0)
			return -4;
		if (pid == 0)
			execlp(programName, "-", NULL);
		waitpid(pid, &res, 0);
		if (res != SUCCESS) {
			fp.unlink_fifo();
			return res;
		}
		char buf[1024];
		if (fp.read_fifo(buf, 1024) != SUCCESS) {
			fp.unlink_fifo();
			return -2;
		}
		if (fp.unlink_fifo() != SUCCESS) 
			return -3;
		std::cout << buf << std::endl;
		return 0;
	} else if (argc == 1) {
		pipc::fifo fc(fifo_name, false, O_WRONLY);
		if (fc.setup() != SUCCESS)
			exit(-1);
		char hello[] = "hello world!";
		if (fc.write_fifo(hello, 13) != SUCCESS)
			exit(-2);
		exit(0);
	}
	return 0;
}
