#include <iostream>
#include <sys/wait.h>
#include <pipc/socket/socket.hpp>

int func(char* in, char* out) {
	strcpy(out, in);
	return 0;
}

int main(int argc, char* argv[]) {
	std::cout << INADDR_LOOPBACK << std::endl;
	int address = INADDR_LOOPBACK;
	if (argc > 1)
		address = std::atoi(argv[1]);
	int port = 8080;
	pipc::socket_server s(port, address);
	int r = s.setup();
	if (r != SUCCESS) {
		std::cerr << "failed server setup " << r << std::endl;
		perror("failed");
		return -1;
	}
	pipc::socket_client c(port, "127.0.0.1");
	pid_t pid = fork();
	if (pid == 0) {
		if (c.setup() != SUCCESS)
			exit(-1);
		char hello[] = "hello world";
		char buf[1024];
		std::cout << "exchange " << c.exchange(hello, 12, buf, 1024) << std::endl;
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

