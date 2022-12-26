#include <iostream>
#include <pipc/process/process.hpp>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 3)
		return -1;
	char* file = argv[1];
	std::string flag = std::string(argv[2]);
	pipc::process p("hello.exe");
	p.set_forward_flag(flag);
    p.forward_stdout(file);
	return 0;
}
