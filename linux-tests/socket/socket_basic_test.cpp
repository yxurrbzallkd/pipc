#include <iostream>
#include <sys/wait.h>
#include <pipc/socket/socket.hpp>

int func(char* in, char* out) {
	strcpy(out, in);
	return 0;
}

int main(int argc, char* argv[]) {
	int address = INADDR_ANY;
	if (argc > 1)
		address = std::atoi(argv[1]);
	int port = 8080;
	if (argc > 2)
		port = std::atoi(argv[2]);
	pipc::socket_server s(port, address);
	int r = s.setup();
	if (r != SUCCESS)
		return -1;
	
	pipc::socket_client c(port, address);
	pid_t pid = fork();
	if (pid == 0) {
		if (c.setup() != SUCCESS)
			exit(-1);
		char hello[] = "hello world";
		char buf[1024];
		if (c.exchange(hello, 12, buf, 1024) != SUCCESS)
			return -1;
		std::cout << buf << std::endl;
		s.set_done();
		exit(0);
	} else {
		char buf[1024];
		s.run_read_write(&func);
		waitpid(pid, NULL, NULL);
		s.interrupt();
	}
	return 0;
}

