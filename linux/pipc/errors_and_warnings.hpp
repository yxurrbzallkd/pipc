#ifndef ERROR_WARNING_HEADER
#define ERROR_WARNING_HEADER

#include <string>
#include <iostream>
#include <math.h>

#define PROCESS_ERROR  -1 * (2 << 29)
#define FIFO_ERROR     -1 * (2 << 28)
#define SHM_ERROR      -1 * (2 << 27)
#define PIPE_ERROR     -1 * (2 << 26)
#define FORWARD_ERROR  -1 * (2 << 25)

#define ARGUMENT_ERROR        1
#define NOT_SETUP             2
#define FAILED_TO_PIPE        4
#define PROCESS_FAILED        8
#define FAILED_TO_FORK        16
#define FAILED_TO_OPEN        32
#define FAILED_TO_CLOSE       64
#define INVALID_FD            128
#define INVALID_FLAG          256
#define FAILED_TO_DUP         512
#define FAILED_TO_UNLINK     1024
#define FAILED_TO_MKFIFO     2048
#define FAILED_TO_MMAP       4096
#define FAILED_TO_MUNMAP     8192
#define FAILED_TO_FTRUNCATE  1024 + 2
#define OUT_OF_BOUNDS        4096 + 2
#define BAD_WRITE            8192 + 2
#define BAD_READ             2 << 13
#define SOCKET_ERROR         2 << 14
#define FAILED_TO_SETSOCKOPT 2 << 15
#define FAILED_TO_BIND       2 << 16
#define FAILED_TO_LISTEN     2 << 17
#define FAILED_TO_ACCEPT     2 << 18
#define FAILED_TO_SEND       2 << 19
#define BAD_ADDRESS          2 << 20
#define FAILED_TO_CONNECT    2 << 21
#define BUSY                 2 << 22
#define SUCCESS 0


int error_return(std::string message) {
	int errcode = errno;
	std::cerr << "ERROR: " << message << std::endl;
	return errcode;
}

int error_exit(std::string message) {
	perror(message.c_str());
	exit(errno);
}

void warn(std::string warning) {
	std::cerr << "WARNING: " << warning << std::endl;
}

#endif