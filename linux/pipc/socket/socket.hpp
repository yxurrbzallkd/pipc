#ifndef SOCKET_HEADER
#define SOCKET_HEADER
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <pipc/errors_and_warnings.hpp>

void sighandler(int signo){
    if(signo == SIGUSR1)
		exit(SUCCESS);
    return;
}

#define SOCKET_BUF_SIZE 2048
#define MESSAGE_BUF_SIZE 65536

namespace pipc {
	class socket_server {
		private:
			struct sockaddr_in serv_info = {0};
			int sfd;
			int PORT;
			int ADDRESS;
			int addrlen;
			bool issetup = false;
			bool isrunning = false;
			pid_t pid;

		public:
			socket_server(int port, int address)
			: PORT(port), ADDRESS(address) {}
			int setup() {
				serv_info.sin_family = AF_INET;
				serv_info.sin_port = htons(PORT);
			    serv_info.sin_addr.s_addr = htonl(ADDRESS);//htonl(INADDR_LOOPBACK);
				addrlen = sizeof(serv_info);
				sfd = socket(AF_INET, SOCK_STREAM, 0);
				if (sfd < 0)
					return SOCKET_ERROR | FAILED_TO_OPEN;
				int x = ADDRESS;
				if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void*)&x, sizeof(x)) < 0) {
					std::cout << "failed to setsockopt" << std::endl;
					return SOCKET_ERROR | FAILED_TO_SETSOCKOPT;
				}
				if (bind(sfd, (struct sockaddr*)&serv_info, sizeof(struct sockaddr_in)) < 0) {
					std::cout << "failed to bind" << std::endl;		
					return SOCKET_ERROR | FAILED_TO_BIND;
				}
				if (listen(sfd, 10) < 0)
					return SOCKET_ERROR | FAILED_TO_LISTEN;
				issetup = true;
				return SUCCESS;
			}
			int run_read_write(int (*func)(char*, char*)) {
				if (!issetup)
					return SOCKET_ERROR | NOT_SETUP;
				pid = fork();
				if (pid < 0)
					return SOCKET_ERROR | FAILED_TO_FORK;
				struct sigaction ccatch;
				sigemptyset(&ccatch.sa_mask);   /* ccatch for catching signal from parent */
			    ccatch.sa_flags = 0;
			    ccatch.sa_handler = sighandler;
				sigaction(SIGUSR1, &ccatch, NULL); /* catch signal from parent for child */
				isrunning = true;
				if (pid == 0) {
					int accepted = 0;
					int success = 0;
					int cfd;
					char inbuf[MESSAGE_BUF_SIZE+1];
					char outbuf[MESSAGE_BUF_SIZE+1];
					while (isrunning) {
						cfd = accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
						if (cfd > 0) {
							accepted += 1;
							int r = read(cfd, inbuf, MESSAGE_BUF_SIZE);
							if (r >= 0) {
								inbuf[r] = '\0';
								func(inbuf, outbuf);
								std::cout << "received " << inbuf;
								std::cout << " sent " << outbuf << std::endl;
								if (write(cfd, outbuf, MESSAGE_BUF_SIZE) >= 0)
									success += 1;
							}
							close(cfd);
						}
					}
				}
				return SUCCESS;
			}

			int interrupt() {
				if (!issetup)
					return SOCKET_ERROR | NOT_SETUP;
				if (!isrunning)
					return SUCCESS;
				kill(pid, SIGUSR1);
				isrunning = false;
				return SUCCESS;
			}

			int accept_read(char* buf, size_t n) {
				if (!issetup)
					return SOCKET_ERROR | NOT_SETUP;
				if (isrunning)
					return SOCKET_ERROR | BUSY;
				isrunning = true;
				while (isrunning) {
					int cfd = accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
					//if (cfd < 0)
					//	return SOCKET_ERROR | FAILED_TO_ACCEPT;
					if (cfd > 0) {
						std::cout << "accepted" << std::endl;
						int r = read(cfd, buf, n);
						if (r < 0)
							return SOCKET_ERROR | BAD_READ;
						if (close(cfd) < 0)
							return SOCKET_ERROR | FAILED_TO_CLOSE;
						break;
					}
				}
				std::cout << "not running" << std::endl;
				isrunning = false;
				return SUCCESS;
			}

			int accept_read_write(char* buf, int func(char*, char*)) {
				if (!issetup)
					return SOCKET_ERROR | NOT_SETUP;
				if (isrunning)
					return SOCKET_ERROR | BUSY;
				isrunning = true;
				while (isrunning) {
					int cfd = accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
					if (cfd > 0) {
						int r = read(cfd, buf, MESSAGE_BUF_SIZE);
						if (r < 0)
							return SOCKET_ERROR | BAD_READ;
						char outbuf[MESSAGE_BUF_SIZE];
						if (func(buf, outbuf) != 0)
							return SOCKET_ERROR;
						if (write(cfd, outbuf, MESSAGE_BUF_SIZE) < 0)
							return SOCKET_ERROR | BAD_WRITE;
						if (close(cfd) < 0)
							return SOCKET_ERROR | FAILED_TO_CLOSE;
						break;
					}
				}
				isrunning = false;
				return SUCCESS;
			}

			int set_done() {
				isrunning = false;
				return SUCCESS;
			}

			int get_port() {
				return PORT;
			}

			~socket_server() {
				close(sfd);
			}

	};

	class socket_client {
		private:
			struct sockaddr_in cli_info = {0};	
			int sfd;
			int PORT;
			const char* ADDRESS;
			bool issetup = false;
		public:
			socket_client(int port, const char* address)
			: PORT(port), ADDRESS(address) { }

			int setup() {
				cli_info.sin_family = AF_INET;
				cli_info.sin_port = htons(PORT);
				sfd = socket(AF_INET, SOCK_STREAM, 0);
				if (sfd < 0)
					return SOCKET_ERROR | FAILED_TO_OPEN;
				if (inet_pton(AF_INET, ADDRESS, &cli_info.sin_addr)<=0)
					return SOCKET_ERROR | BAD_ADDRESS;
				if (connect(sfd, (struct sockaddr *)&cli_info, sizeof(cli_info)) < 0)
					return FAILED_TO_CONNECT;
				issetup = true;
				return SUCCESS;
			}
			int exchange(char* msg, size_t s, char* buf, size_t a) {
				if (!issetup)
					return SOCKET_ERROR | NOT_SETUP;
				if (send(sfd, msg, s, 0) < 0)
					return SOCKET_ERROR | FAILED_TO_SEND;
				if (read(sfd, buf, a) < 0)
					return SOCKET_ERROR | BAD_READ;
				return SUCCESS;
			}
			~socket_client() {
				std::cout << "killing" << std::endl;
				close(sfd);
			}
	};
}

#endif

