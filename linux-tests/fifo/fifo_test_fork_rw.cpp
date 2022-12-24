#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc > 1)
		fifo_name = argv[1];
	
	pipc::fifo fp(fifo_name, true, O_RDWR);
	if (fp.setup() != SUCCESS) return -1;
	
	pid_t pid = fork();
	if (pid < 0)
		return -1;
	if (pid == 0) {
		pipc::fifo fc(fifo_name, false, O_WRONLY);
		if (fc.setup() != SUCCESS) return -1;
		char hello[13] = "hello world!";
		int res = fc.write_fifo(hello, 13);
		if (res != SUCCESS) return -1;
		exit(res);
	}
	int status;
	waitpid(pid, &status, 0);
	char buf[1024];
	if (fp.read_fifo(buf, 13) != SUCCESS) {
		fp.unlink_fifo();
		return -1;
	}
	if (fp.unlink_fifo() != SUCCESS) return -1;
	std::cout << buf << std::endl;
	return 0;
}
