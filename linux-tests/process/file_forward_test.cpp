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
	p.set_forward_flag(flag);
	p.run_exec(0, file, STDERR_FILENO);
	std::pair<string, string> r = p.run_grab(STDIN_FILENO);
	std::cout << r.first << " " << r.second << " " << p.get_result() << std::endl;
	return 0;
}

	