#ifndef ERROR_WARNING_HEADER
#define ERROR_WARNING_HEADER

#include <string>
#include <iostream>

#define PROCESS_ERROR     -2 // 2*1
#define FILE_ERROR        -4 // 2**2
#define PIPE_ERROR        -8 // ...
#define FORWARD_ERROR    -16
#define ARGUMENT_ERROR   -32
#define PROCESS_FAILED   -64
#define FAILED_TO_FORK  -128
#define FAILED_TO_OPEN  -256
#define INVALID_FD      -512
#define INVALID_FLAG   -1024
#define FAILED_TO_PIPE -2048
#define FAILED_TO_DUP  -4096
#define FAILED_TO_CLOSE -8192

#define NOT_SETUP           -16384 // 2**14
#define FAILED_TO_UNLINK    -23768 // 2**15
#define FIFO_ERROR          -65536 // ...
#define FAILED_TO_MKFIFO   -131072
#define SHM_ERROR          -262144
#define FAILED_TO_MMAP     -524288
#define FAILED_TO_MUNMAP   -104856
#define FAILED_TO_FTRUNCATE -2097152 
#define OUT_OF_BOUNDS       -4194304
#define BAD_WRITE           -8388608
#define BAD_READ           -16777216

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