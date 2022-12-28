#include <iostream>
#include <pipc/fifo/fifo.hpp>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc > 1)
		fifo_name = argv[1];
	pipc::fifo fc(fifo_name, false, O_WRONLY);
	if (fc.setup() != SUCCESS)
		exit(-1);
	char hello[] = "hello world!";
	if (fc.write_fifo(hello, 13) != SUCCESS)
		exit(-2);
	exit(0);
	return 0;
}
