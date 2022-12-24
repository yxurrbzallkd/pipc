#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	char program_name[] = "./fifo_test_wr";
	if (argc == 2) {
		// IMPORTANT!!! Pass O_RDWR else HANGS
		pipc::fifo fp(fifo_name, true, O_RDWR);
		if (fp.setup() != SUCCESS) return -1;
		
		char hello[13] = "hello world!";
		if (fp.write_fifo(hello, 13) != SUCCESS) {
			fp.unlink_fifo();
			return -2;
		}
		int res = execlp(program_name, program_name, NULL);
		wait(NULL);
		if (fp.unlink_fifo() != SUCCESS) return -3;
		if (res != SUCCESS) return res;
	} else {
		pipc::fifo fc(fifo_name, false, O_RDONLY);
		if (fc.setup() != SUCCESS) exit(-1);
		char buf[1024];
		if (fc.read_fifo(buf, 1024) != SUCCESS) exit(-2);
		std::cout << buf << std::endl;
		exit(0);
	}
	return 0;
}
