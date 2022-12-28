#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	char* program_name = (char*)"./wr_child";
	if (argc > 1)
		fifo_name = argv[1];
	if (argc > 2)
		program_name = argv[2];
	pipc::fifo fp(fifo_name, true, O_RDWR);
	if (fp.setup() != SUCCESS) return -1;

	char hello[13] = "hello world!";
	if (fp.write_fifo(hello, 13) != SUCCESS)
		return -2;
		
	pid_t pid = fork();
	if (pid < 0)
		return -4;
	int res;
	if (pid == 0) {
		execlp(program_name, program_name, NULL);
	} else {
		waitpid(pid, &res, 0);
	}
	if (res != SUCCESS) return res;
	return 0;
}
