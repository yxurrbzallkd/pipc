# PIPC Linux

## ```pipc/process```

### ```process_base.hpp```
```c++
namespace pipc {
	class basic_process {
		protected:
			string program;
			vector<string> arguments;
			int fd_in, fd_out, fd_err;
			string out_flag, err_flag;
			bool isexec = false;
			int result;
			char buf[BUF_SIZE+1]; // for reuse

			void _reset_fds_flags()
				// restore default values
			void _parse_command(vector<string> args)
				/*
				looks for forwarding symbols and respective files
				cuts them out, forwards stdin\out\err
				*/
			int _dup_all()
				/* replaces stdin\out\err with forwarding fd's */
			void _close_all()
				// closes fd's if not standard used
			string reader(int fd)
				// reads from fd and combines into string

		public:
			basic_process(string command)
				// example "ls -l dir"
			basic_process(string path, vector<string> args)
				// Example: ls, {"-l", "dir"}
			basic_process(vector<string> args)
				// Example: {"ls", "-l", "dir"}
			
			int set_forward_flag(string flag)

			int forward_stdin(int in) {
				// fd provided
				if (in == STDIN_FILENO)
					return SUCCESS;
				int fd = dup(in); // make a duplicate for private use
				if (fd < 0)
					return FORWARD_ERROR | FAILED_TO_DUP;
				if (fcntl(fd, F_SETFL, O_RDONLY) < 0)
					return FORWARD_ERROR | INVALID_FLAG;
				fd_in = fd;
				return SUCCESS;
			}

			int forward_stdin(const char* in)
				// char* - file provided
			int forward_stdout(int out)
				// provide fd
			int forward_stderr(int out)
				// provide fd
			int forward_stderr(const char* out)
				// provide filename
			std::vector<string> get_args()
				// return argument
			int get_result()
				// return result of execution
			std::string get_command()
				// returns command
	}
}
```

### ```process.hpp```
```c++
#ifndef PROCESS_HEADER
#define PROCESS_HEADER
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>
#include "process_helpers.hpp"
#include "process_base.hpp"
#include <pipc/errors_and_warnings.hpp>

#define BUF_SIZE 1024

using namespace std;

namespace pipc {
	class process : public basic_process {
		private:
			int _grab_execute(std::pair<string, string>& res)
				/*
				creates 2 pipes for stdout and stderr
				forks
				dup2's child's stdout\err to reading ends of pipes
				reads from pipes in parent
				stores read from stdout and stderr in res.first and res.second respectively
				*/
			int _execute()
				/*
				forks
				dup2's stdin/out/err
				calls execvp
				waits in parent
				returns error code
				*/

		public:
			using basic_process::basic_process;

			template <typename IN, typename OUT, typename ERR>
			int run_exec(IN in, OUT out, ERR err) {
				/*
				run the proces
				accepts fd's and filenames
				pass standard if don't want to forward
				*/
				result = forward_stdin(in);
				if (result < 0) return result;
				result = forward_stdout(out);
				if (result < 0) return result;
				result = forward_stderr(err);
				if (result < 0) return result;
				result = _execute();
				return result;
			}
			int run_exec() {
				// without arguments
				result = _execute();
				return result
			}
			std::pair<string, string> run_grab()
				/*
				grabs stdout and stderr
				stores stdout in pair's first, stderr in second
				*/
	};
}
```

### ```background.hpp```
DOESN'T WORK

### other
```process_helpers.hpp``` - command parsing, opening files etc.

## ```pipe.hpp```
```c++
namespace pipc {
	int pipe_execute(std::vector<pipc::process> ps) {
		// conveyor
		// !!! blocks if writes more than size of pipe
		if (ps.size() < 2) // at least 2 processes
			return ARGUMENT_ERROR;

		int n = ps.size();
		int pipes[n-1][2];
		for (int i = 0; i < n-1; i++)
			// pipe()

		ps[0].forward_stdout(pipes[0][1]);
		if (ps[0].run_exec() < 0)
			return PROCESS_ERROR | PROCESS_FAILED;
		for (int i = 1; i < n-1; i++) {
			close(pipes[i-1][1]);
			ps[i].forward_stdin(pipes[i-1][0]);
			ps[i].forward_stdout(pipes[i][1]);
			if (ps[i].run_exec() < 0)
				return PROCESS_ERROR | PROCESS_FAILED;
			close(pipes[i-1][0]);
		}
		// last process - don't forward stdout
		close(pipes[n-2][1]);
		ps[n-1].forward_stdin(pipes[n-2][0]);
		if (ps[n-1].run_exec() < 0)
			return PROCESS_ERROR | PROCESS_FAILED;
		close(pipes[n-2][0]);
		return n;
	}
}
```

## Fifo
```c++
namespace pipc {
	class fifo {
		private:
			const char* fifo_name;
			bool fifo_create;
			int fifo_flag;
			int fifo_fd;
			int buf[BUF_SIZE];
			bool issetup = false;
		public:
			fifo(const char* name, int flag) : fifo_name(name), fifo_flag(flag)
			{ fifo_create = true; }
			fifo(const char* name, bool create, int flag) : fifo_name(name), fifo_create(create), fifo_flag(flag)
			{ }
			int setup();
			int write_fifo(char* buf, size_t size);
			int read_fifo(char* buf, size_t n);
			int clear_fifo() {
				// read everything from the pipe, clearing it
				while (read(fifo_fd, buf, BUF_SIZE) > 0) {}
				return SUCCESS;
			}
			~fifo() // close \ unlink
	};
}
```

## Shmem
```c++
#ifndef SHMEM_HEADER
#define SHMEM_HEADER

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pipc/errors_and_warnings.hpp>

namespace pipc {
	class shmem {
		private:
			char* shm_name;
			int sfd;
			int shm_mode, map_flag;
			char* map;
			size_t file_size;
			bool shm_create;
			bool issetup = false;
		public:
			shmem(char* name, size_t size, int sflag, int mflag)
			: shm_name(name), shm_mode(sflag), file_size(size), map_flag(mflag)
			{ shm_create = true; }

			shmem(char* name, int sflag, int mflag)
			: shm_name(name), shm_mode(sflag), map_flag(mflag)
			{ shm_create = false; }


			int setup() {
				// shm_flag is different in this context
				int flag = 0;
				if (shm_mode == O_RDWR)
					flag = S_IRUSR | S_IWUSR;
				else if (shm_mode == O_RDONLY)
					flag = S_IRUSR;
				else if (shm_mode == O_WRONLY)
					flag = S_IWUSR;
				else
					return SHM_ERROR | INVALID_FLAG;
				shm_mode = flag;
				if (shm_create)
					sfd = sfd = shm_open(shm_name, O_CREAT | O_RDWR, shm_mode);
				else
					sfd = shm_open(shm_name, O_RDWR, shm_mode);
				// ...
				if (shm_create) {
					// ftruncate
				else
					file_size = lseek(sfd, 0, SEEK_END);
				
				// mmap
				// ...
			}

			int write_shm(char* buf, size_t n, size_t offset)
			int read_shm(char* buf, size_t n, size_t offset)
			int resize_shm(size_t new_size) {
				// ftruncate
				// munmap
				// mmap
			}
			~shmem() // clos - unlink
	};
}
```

## Socket
```c++
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
				// setup serv_info
				socket(AF_INET, SOCK_STREAM, 0);
				// ...
				setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void*)&x, sizeof(x))
				bind(sfd, (struct sockaddr*)&serv_info, sizeof(struct sockaddr_in)) < 0)
				// ...
				listen(sfd, 10) < 0)
				// ...
			}
			int run_read_write(int (*func)(char*, char*)) {
				// ...
				// fork()
				// setup sigaction ccatch
				struct sigaction ccatch;
				// ...
				sigaction(SIGUSR1, &ccatch, NULL); /* catch signal from parent for child */
				// ...
				if (pid == 0) {
					// ...
					while (isrunning) {
						cfd = accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
						// read(cfd, inbuf, MESSAGE_BUF_SIZE);
						func(inbuf, outbuf);
						// write(cfd, outbuf, MESSAGE_BUF_SIZE)
						close(cfd);
					}
				}
				return SUCCESS;
			}

			int interrupt() {
				// ...
				kill(pid, SIGUSR1);
				isrunning = false;
				return SUCCESS;
			}

			int accept_read(char* buf, size_t n) {
				// ...
				isrunning = true;
				while (isrunning) {
					int cfd = accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
					// int r = read(cfd, buf, n);
					// close(cfd)
					// break
					// ...
				}
				// ...
			}

			int accept_read_write(char* buf, int func(char*, char*)) {
				// ...
				isrunning = true;
				while (isrunning) {
					int cfd = accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
					// ...
					// int r = read(cfd, buf, MESSAGE_BUF_SIZE);
					// func(buf, outbuf)
					// write(cfd, outbuf, MESSAGE_BUF_SIZE)
					// close(cfd)
					// break
				}
				// ...
			}

			int set_done() {
			int get_port()
			~socket_server() // close
	};

	class socket_client {
		private:
			struct sockaddr_in cli_info = {0};	
			int sfd;
			int PORT;
			const char* CHAR_ADDRESS = NULL;
			int INT_ADDRESS = NULL;
			bool issetup = false;
		public:
			socket_client(int port, const char* address)
			: PORT(port), CHAR_ADDRESS(address) { }

			socket_client(int port, int address)
			: PORT(port), INT_ADDRESS(address) { }

			int setup() {
				// initialize cli_info
				// socket(AF_INET, SOCK_STREAM, 0);
				if (CHAR_ADDRESS != NULL)
					if (inet_pton(AF_INET, CHAR_ADDRESS, &cli_info.sin_addr)<=0)
						return SOCKET_ERROR | BAD_ADDRESS;
				else
					cli_info.sin_addr.s_addr = htonl(INT_ADDRESS);
				// connect(sfd, (struct sockaddr *)&cli_info, sizeof(cli_info))
				// ...
			}
			int exchange(char* msg, size_t s, char* buf, size_t a) {
				// send(sfd, msg, s, 0) < 0)
				// read(sfd, buf, a) < 0)
				// ...
			}
			~socket_client() // close
	};
}
```

*signal in socket*

```c++
        int run_read_write(int (*func)(char*, char*)) {
            // ...
            pid = fork();
			// ...
            struct sigaction ccatch;
            sigemptyset(&ccatch.sa_mask);   /* ccatch for catching signal from parent */
            ccatch.sa_flags = 0;
            ccatch.sa_handler = sighandler;
            sigaction(SIGUSR1, &ccatch, NULL); /* catch signal from parent for child */
           
            isrunning = true;
            if (pid == 0) {
				/...
                while (isrunning) {
                    // accept(sfd, (struct sockaddr*)&serv_info, (socklen_t*)&addrlen);
                    // int r = read(cfd, inbuf, MESSAGE_BUF_SIZE);
                    // func(inbuf, outbuf);
                    // write(cfd, outbuf, MESSAGE_BUF_SIZE)
                    // close(cfd);
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
```

