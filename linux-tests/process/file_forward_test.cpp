#include <iostream>
#include <pipc/process/process.hpp>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 1)
		error_exit("provide arguments");
	std::vector<std::string> v(argv, argv + argc);
	pipc::process p(v);
	p.run_exec(0, "file.txt", STDERR_FILENO);
	std::pair<string, string> r = p.run_grab(STDIN_FILENO);
	std::cout << r.first << " " << r.second << " " << p.get_result() << std::endl;
	return 0;
}

	