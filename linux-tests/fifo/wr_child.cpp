#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc > 1)
		fifo_name = argv[1];
	pipc::fifo fc(fifo_name, false, FF_RD);
	if (fc.setup() != SUCCESS) exit(-1);
	char buf[1024];
	if (fc.read_fifo(buf, 1024) != SUCCESS) exit(-2);
	std::cout << buf << std::endl;
	exit(0);
	return 0;
}
