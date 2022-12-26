#include <iostream>
#include <pipc/process/process.hpp>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 1)
		return -1;
	std::vector<std::string> v(argv+1, argv + argc);
	pipc::process p(v);
	p.run_exec();
	return GetLastError();
}
