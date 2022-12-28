#include <iostream>
#include <pipc/process/process.hpp>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 1 + 2 + 1) // program + forward flag + forward file
		error_exit("provide arguments");
	std::vector<std::string> v(argv+1, argv + argc-2);
	std::string flag = argv[argc-2];
	char* file = argv[argc-1];
	pipc::process p(v);
	if (p.set_forward_flag(flag) != SUCCESS)
		return -1;
	if (p.forward_stdout(file) != SUCCESS)
		return -2; 
	p.run_exec();
	return p.get_result();
}

	