#include <pipc/process/process.hpp>

int main(){
	// Process creatino
	pipc::process p("ls -l dir"); // string
	pipc::process p("ls", {"-l", "dir"}); // string + vector
	pipc::process p({"ls", "-l", "dir"}); // vector
	// running
	p.run_exec();
	// forwarding 1
	int fd = open("file.txt");
	p.forward_stdout(fd);
	// forwarding 2
	fd.forward_stdout("file.txt");
	// forwarding 3 and 4
	p.run_exec(0, "file.txt", 2);
	p.run_exec(0, fd, 2);
	// grab output, first - stdout, second - stderr
	std::pair<std::string, std::string> = p.run_grab();
	// conveyor
	pipc::pipe_execute({p1, p2, p3});

	return 0;
}