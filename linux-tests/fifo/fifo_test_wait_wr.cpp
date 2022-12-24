#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc > 1)
		fifo_name = argv[1];
	pid_t pid = fork();
	if (pid < 0)
		return -1;
	if (pid == 0) {
		pipc::fifo fc(fifo_name, false, O_RDONLY);
		sleep(1); // give parent time to setup the fifo
		if (fc.setup() != SUCCESS) exit(-1);
		char buf[1024];
		sleep(1); // give parent time to write
		if (fc.read_fifo(buf, 1024) != SUCCESS) exit(-1);
		std::cout << buf << std::endl;
		exit(0);
	} else {
		pipc::fifo fp(fifo_name, true, O_WRONLY);
		if (fp.setup() != SUCCESS) return -1;
		char hello[13] = "hello world!";
		if (fp.write_fifo(hello, 13) != SUCCESS) {
			fp.unlink_fifo();
			return -1;
		}
		int status;
		waitpid(pid, &status, 0);
		if (fp.unlink_fifo() != SUCCESS) return -1;
	}
	return 0;
}
