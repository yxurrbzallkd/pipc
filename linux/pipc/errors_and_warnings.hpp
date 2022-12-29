#ifndef ERROR_WARNING_HEADER
#define ERROR_WARNING_HEADER

#include <string>
#include <iostream>
#include <math.h>

#define ARGUMENT_ERROR -(int)pow(2, 30)

#define PROCESS_ERROR  -(int)pow(2, 29)

#define FIFO_ERROR     -(int)pow(2, 28)

#define SHM_ERROR      -(int)pow(2, 27)

#define NOT_SETUP      -(int)pow(2, 26) // 2**14

#define PIPE_ERROR     -(int)pow(2, 25)
#define FAILED_TO_PIPE -(int)pow(2, 24)

#define FORWARD_ERROR    -(int)pow(2, 23)
#define PROCESS_FAILED   -(int)pow(2, 22)

#define FAILED_TO_FORK  -(int)pow(2, 21)

#define FAILED_TO_OPEN  -(int)pow(2, 20)
#define FAILED_TO_CLOSE -(int)pow(2, 19)

#define INVALID_FD      -(int)pow(2, 18)
#define INVALID_FLAG    -(int)pow(2, 17)

#define FAILED_TO_DUP  -(int)pow(2, 16)

#define FAILED_TO_UNLINK    -(int)pow(2, 15)

#define FAILED_TO_MKFIFO   -(int)pow(2, 14)

#define FAILED_TO_MMAP      -(int)pow(2, 15)
#define FAILED_TO_MUNMAP    -(int)pow(2, 14)
#define FAILED_TO_FTRUNCATE -(int)pow(2, 13)

#define OUT_OF_BOUNDS       -(int)pow(2, 12)

#define BAD_WRITE           -(int)pow(2, 11)
#define BAD_READ            -(int)pow(2, 10)

#define SOCKET_ERROR          -(int)pow(2, 9)
#define FAILED_TO_SETSOCKOPT  -(int)pow(2, 8)
#define FAILED_TO_BIND        -(int)pow(2, 7)
#define FAILED_TO_LISTEN      -(int)pow(2, 6)
#define FAILED_TO_ACCEPT      -(int)pow(2, 5)
#define FAILED_TO_SEND        -(int)pow(2, 4)
#define BAD_ADDRESS           -(int)pow(2, 3)
#define FAILED_TO_CONNECT     -(int)pow(2, 2)
#define BUSY                  -(int)pow(2, 1)

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