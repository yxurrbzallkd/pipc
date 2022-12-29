#ifndef HELPER_HEADER
#define HELPER_HEADER

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
#include <pipc/errors_and_warnings.hpp>

int open_file(std::string fpath, int flags);
char** vector_to_chararr(const std::vector<std::string>& args);

namespace pipc {
    int _forward_outerr_helper(int out, std::string flag) {
		// was out fd opened with proper flags?
		// if yes, make a copy of it, for provate use?
		// could be removed, really...
		if (out < 0)
			return INVALID_FD;
		int fdout = dup(out); // make a private duplicate
		if (fdout < 0)
			return FAILED_TO_DUP;
		if (fcntl(fdout, F_SETFL, W_OK) < 0)
			return INVALID_FLAG; 
		return fdout;
	}

    int _string_to_flag(std::string how) {
    	int flag;
    	if (how == ">>" || how == "1>>" || how == "2>>")
    		flag = O_APPEND | W_OK;
    	else if ((how == ">") || (how == "1>") || (how == "2>"))
    		flag = O_CREAT | W_OK | R_OK;
    	else
    		flag = R_OK;
    	return flag;
    }

    int _forward_open_helper(const char* out, std::string flag) {
		int fd = open(out, _string_to_flag(flag), 0666); // 0666 permissions
		if (fd < 0) return PROCESS_ERROR | FORWARD_ERROR | fd;
		return fd;
    }

    bool _forward_parse(std::string s,
						std::vector<std::string>& args,
                        std::pair<std::string, std::string>& wh) {
		std::vector<std::string>::iterator it;
        bool res = false;
		it = std::find(args.begin(), args.end(), s);
		if (it != args.end()) {
			if (next(it, 1) != args.end()) {
				wh.first = *it; wh.second = *next(it, 1);
				args.erase(it+1);
				res = true;
			} else
				warn(s+" used, file not specified");
			args.erase(it);
		}
		return res;
	}
}

int open_file(std::string fpath, int flags) {
	int fd = open(fpath.c_str(), flags);
	if (fd < 0) return FAILED_TO_OPEN;
	return fd;
}

char** vector_to_chararr(const std::vector<std::string>& args) {
	char** argv = (char**) malloc(sizeof(char*)*(args.size()+1));
    for (int i = 0; i < args.size(); i++)
        argv[i] = (char*)args[i].c_str();
    argv[args.size()] = NULL;
	return argv;
}

#endif