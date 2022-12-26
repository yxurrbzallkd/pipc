#ifndef ERROR_WARNING_HEADER
#define ERROR_WARNING_HEADER

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#define PROCESS_ERROR                (int)pow(2, 31)    // 1000 ...
#define FORWARD_ERROR                (int)pow(2, 30)
#define FAILED_SET_HANDLE_INFO       (int)pow(2, 27)
#define FAILED_DUPLICATE_HANDLE      (int)pow(2, 26)
#define FAILED_TO_FIND_FILE          (int)pow(2, 25)
#define FAILED_TO_CREATE_PROCESS     (int)pow(2, 24)
#define BAD_FLAG                     (int)pow(2, 23)
#define FAILED_GET_HANDLE_INFO       (int)pow(2, 22)
#define BAD_HANDLE                   (int)pow(2, 22)
#define FAILED_TO_CREATE_FILE        (int)pow(2, 21)
#define SHM_ERROR                    (int)pow(2, 29)
#define FAILED_TO_CREATE_FILE_MAPPING (int)pow(2, 20)
#define FAILED_TO_OPEN_MAPPING       (int)pow(2, 19)
#define FAILED_TO_MAP                (int)pow(2, 18)
#define NOT_SETUP                    (int)pow(2, 17)
#define FIFO_ERROR                   (int)pow(2, 28)
#define FAILED_TO_CREATE_PIPE        (int)pow(2, 16)
#define FAILED_TO_CONNECT            (int)pow(2, 15)
#define BAD_READ                     (int)pow(2, 14)
#define BAD_WRITE                    (int)pow(2, 13)
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