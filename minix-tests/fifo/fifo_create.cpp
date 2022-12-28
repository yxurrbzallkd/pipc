#include <iostream>
#include <pipc/fifo/fifo.hpp>

using namespace std;

int main(int argc, char* argv[]) {
	char * fifo_name = (char*)"/tmp/fifo";
	if (argc > 1)
		fifo_name = argv[1];
	char* program_name = argv[0];
	pipc::fifo fp(fifo_name, true, O_RDWR);
	if (fp.setup() != SUCCESS) return -1;
	return 0;
}
