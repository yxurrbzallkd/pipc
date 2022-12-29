#ifndef HELPER_HEADER
#define HELPER_HEADER
#include <windows.h> 
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>
#include "vector_to_chararr.hpp"
#include <pipc/errors_and_warnings.hpp>
#include <system_error>
#include <tchar.h>
#include <shlobj.h>
#include <cstdio>
#include <strsafe.h>
#include <string.h>


#define BUF_SIZE 1024

using namespace std;

namespace pipc {
	bool _findFile(CHAR* startDirectory, CHAR* fileToFind, CHAR* foundFile, bool recurse) {
		WIN32_FIND_DATAA data;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		CHAR pathBuffer[MAX_PATH];

		StringCchPrintfA(pathBuffer, MAX_PATH, startDirectory);
		StringCchCatA(pathBuffer, MAX_PATH, "\\*");
		hFind = FindFirstFileA(pathBuffer, &data);

		if (INVALID_HANDLE_VALUE == hFind)
			return false;
		
		do
		{
			if (strcmp(data.cFileName, fileToFind) == 0) {
				//printf("File found at: %s\n", pathBuffer);
				CHAR newPath[MAX_PATH];
				StringCchPrintfA(newPath, MAX_PATH, startDirectory);
				StringCchCatA(newPath, MAX_PATH, "\\");
				StringCchCatA(newPath, MAX_PATH, data.cFileName);
				StringCchPrintfA(foundFile, MAX_PATH, newPath);
				return true;
			}
			if (recurse && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
				// check for . and .. links
				if ( (strcmp(data.cFileName, ".") != 0) && (strcmp(data.cFileName,  "..") != 0) ) {
					//printf("Found: %s\n", data.cFileName);
					// modify buffer to have it be the starting point for the recursive call
					CHAR newPath[MAX_PATH];
					StringCchPrintfA(newPath, MAX_PATH, startDirectory);
					StringCchCatA(newPath, MAX_PATH, "\\");
					StringCchCatA(newPath, MAX_PATH, data.cFileName);
					if (_findFile(newPath, fileToFind, foundFile, recurse))
						return true;
				}
			}
		}
		while (FindNextFileA(hFind, &data) != 0);
		DWORD dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			return false;
		}
		FindClose(hFind);
		return false;
	}

	bool findFileCWD(CHAR* fileToFind, CHAR* foundFile) {
		CHAR cwd[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, cwd);
		if (_findFile(cwd, fileToFind, foundFile, false))
			return true;
		return false;
	}

	bool findFile(CHAR* fileToFind, CHAR* foundFile) {
		CHAR windir[MAX_PATH];
		GetWindowsDirectoryA(windir, MAX_PATH);
		if (_findFile(windir, fileToFind, foundFile, false))
			return true;
		StringCchCatA(windir, MAX_PATH, "\\");
		StringCchCatA(windir, MAX_PATH, "System32");
		if (_findFile(windir, fileToFind, foundFile, true))
			return true;
		if (findFileCWD(fileToFind, foundFile))
			return true;
		return false;
	}

	bool _forward_parse(string s,
						vector<string>& args,
						std::pair<string, string>& wh) {
		vector<string>::iterator it; // for reuse
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
	int _string_to_flag(string how) {
		int flag;
		if (how == ">>" || how == "1>>" || how == "2>>")
		{
			flag = FILE_APPEND_DATA;
	    } else if (how == ">" || how == "1>" || how == "2>")
		{
			flag = GENERIC_WRITE;
		}else
			flag = GENERIC_READ;
		return flag;
	}
}
#endif