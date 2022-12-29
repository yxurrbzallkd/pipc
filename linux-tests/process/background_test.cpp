#include <iostream>
#include <pipc/process/background.hpp>

int main(int argc, char* argv[]) {
	pipc::background_process b1("./infinite");
	b1.launch();
	std::cout << b1.is_launched();
	std::cout << b1.is_running();
	std::cout << b1.is_running();
	std::cout << b1.is_finished();
	b1.kill_it();
	std::cout << b1.is_launched();
	std::cout << b1.is_running();
	std::cout << b1.is_finished();
	std::cout << "|";
	pipc::background_process b2("./hello");
	b2.launch();
	std::cout << b2.is_launched();
	std::cout << b2.is_running();
	b2.wait_for();
	std::cout << b2.is_launched();
	std::cout << b2.is_running();
	std::cout << b2.is_finished();
	std::cout << std::endl;
	return 0;
}
