#include <pipc/process/process.hpp>
#include <pipc/fifo/fifo.hpp>

int main(){
	pipc::process p("cmd.exe /c dir");
	p.run_exec();
	return 0;
}