#include <iostream>
#include <pipc/process/process.hpp>

using namespace std;

int main(int argc, char* argv[]) {
	pipc::process p1("cmd.exe /c echo bin");
	pipc::process p2("cmd.exe /c dir");
	pipc::cascade_execute(p1, p2);
	return GetLastError();
}
