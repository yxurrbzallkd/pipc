#ifndef FIFO_HEADER
#define FIFO_HEADER

#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pipc/errors_and_warnings.hpp>

#define BUF_SIZE 1024

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

			int setup() {
				if (fifo_create) {
					if (mkfifo(fifo_name, S_IRUSR|S_IWUSR) < 0)
						return FIFO_ERROR | FAILED_TO_MKFIFO;
				} else {
					if (mkfifo(fifo_name, S_IRUSR|S_IWUSR) >= 0) {
						unlink(fifo_name);
						return FIFO_ERROR | FAILED_TO_MKFIFO;
					}
				}
				if (fifo_create)
					fifo_flag = O_RDWR;
				fifo_fd = open(fifo_name, fifo_flag);
				if (fifo_fd < 0) {
					if (fifo_create) unlink(fifo_name);
					return FIFO_ERROR | FAILED_TO_OPEN;
				}
				issetup = true;
				return SUCCESS;
			}

			int write_fifo(char* buf, size_t size) {
				if (!issetup)
					return FIFO_ERROR | NOT_SETUP;
				if (write(fifo_fd, buf, size) < 0)
					return FIFO_ERROR | BAD_WRITE;
				return SUCCESS;
			}

			int read_fifo(char* buf, size_t n) {
				if (!issetup)
					return FIFO_ERROR | NOT_SETUP;
				if (read(fifo_fd, buf, n) < 0)
					return FIFO_ERROR | BAD_READ;
				return SUCCESS;
			}

			int clear_fifo() {
				// read everything from the pipe, clearing it
				while (read(fifo_fd, buf, BUF_SIZE) > 0) {}
				return SUCCESS;
			}

			~fifo() {
				close(fifo_fd);
				if (fifo_create)
					unlink(fifo_name);
			}
	};
}

#endif