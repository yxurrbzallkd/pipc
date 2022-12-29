#ifndef PROCESS_HEADER
#define PROCESS_HEADER
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
#include "process_helpers.hpp"
#include <system_error>
#include <tchar.h>
#include <shlobj.h>
#include <cstdio>
#include <strsafe.h>
#include <string.h>


#define BUF_SIZE 1024

using namespace std;

namespace pipc {
	class process {
		private:
			string program;
			vector<string> arguments;
			LPWSTR command;
			PROCESS_INFORMATION piProcInfo;
			STARTUPINFO siStartInfo;
			HANDLE handle_in, handle_out, handle_err;
			string out_flag, err_flag;
			bool isexec = false;
			int result;
			char buf[BUF_SIZE+1];

			void _reset_handles_flags() {
				handle_in = GetStdHandle(STD_INPUT_HANDLE);
				handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
				handle_err = GetStdHandle(STD_ERROR_HANDLE);
				out_flag = ">", err_flag = "2>";
			}

			HANDLE _forward_outerr_helper(HANDLE out, string flag) {
				// was out handle opened with proper flags?
				DWORD lpdwFlags[100];
				if (!GetHandleInformation(out, lpdwFlags))
					return INVALID_HANDLE_VALUE;
				if ((lpdwFlags && _string_to_flag(flag)) == 0)
					return INVALID_HANDLE_VALUE;
				// if yes, make a copy of it, for provate use?
				// could be removed, really...
				return out;
			}

			HANDLE _forward_open_helper(char* out, string flag) {
				HANDLE handle;
				if (_string_to_flag(flag) != FILE_APPEND_DATA) {
					handle = CreateFileA(out,
										_string_to_flag(flag), 
										0,
										NULL, 
										OPEN_EXISTING,
										FILE_ATTRIBUTE_NORMAL, 
										NULL);
					if (handle == INVALID_HANDLE_VALUE) {
						handle = CreateFileA(out,
										_string_to_flag(flag), 
										0,
										NULL, 
										CREATE_NEW,
										FILE_ATTRIBUTE_NORMAL, 
										NULL);
					}
				} else {
					handle = CreateFileA(out,
										_string_to_flag(flag), 
										0, 
										NULL, 
										OPEN_EXISTING, 
										FILE_ATTRIBUTE_NORMAL, 
										NULL);
				}
				return handle;
			}

			void _parse_command(vector<string> args) {
				std::pair<string, string> wh; // for reuse
				for (string s : {">", "1>", ">>", "1>>"})
					if (_forward_parse(s, args, wh))
						forward_stdout(wh.second.c_str());
				for (string s : {"2>", "2>>"})
					if (_forward_parse(s, args, wh))
						forward_stderr(wh.second.c_str());
				if (_forward_parse("&>", args, wh)) {
					out_flag = "1>"; err_flag = "2>";
					forward_stdout(wh.second.c_str());
					forward_stderr(wh.second.c_str());
				}
				if (_forward_parse("&>>", args, wh)) {
					out_flag = "1>>"; err_flag = "2>>";
					forward_stdout(wh.second.c_str());
					forward_stderr(wh.second.c_str());
				}
			}

			int _string_to_flag(string how) {
				int flag;
				if (how == ">>" || how == "1>>" || how == "2>>")
					flag = FILE_APPEND_DATA;
			    else if (how == ">" || how == "1>" || how == "2>")
					flag = GENERIC_WRITE;
				else
					flag = GENERIC_READ;
				return flag;
			}

			int _set_all() {
				if ( ! SetHandleInformation(handle_out, HANDLE_FLAG_INHERIT, 0) )
					return -1;
				if ( ! SetHandleInformation(handle_in, HANDLE_FLAG_INHERIT, 0) )
					return -1;
				if ( ! SetHandleInformation(handle_err, HANDLE_FLAG_INHERIT, 0) )
					return -1;
				ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
				siStartInfo.cb = sizeof(STARTUPINFO);
				HANDLE in, out, err;
				if (!DuplicateHandle( GetCurrentProcess(), handle_out,
					GetCurrentProcess(), &out, 0, TRUE, DUPLICATE_SAME_ACCESS))
					return -1;
				if (!DuplicateHandle( GetCurrentProcess(), handle_in,
					GetCurrentProcess(), &in, 0, TRUE, DUPLICATE_SAME_ACCESS))
					return -1;
				if (!DuplicateHandle( GetCurrentProcess(), handle_err,
					GetCurrentProcess(), &err, 0, TRUE, DUPLICATE_SAME_ACCESS))
					return -1;
				siStartInfo.hStdError = err;
				siStartInfo.hStdOutput = out;
				siStartInfo.hStdInput = in;
				siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
				return 0;
			}

			void _close_all() {
				CloseHandle(siStartInfo.hStdInput);
				CloseHandle(siStartInfo.hStdOutput);
				CloseHandle(siStartInfo.hStdError);
			}

			int _execute(bool reset) {
				isexec = true;
				int res;
				ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
				if (_set_all() < 0)
					return -1;
				
				string cmd = "";
				for (int i = 0; i < arguments.size(); i++)
					cmd += " "+arguments[i];
				
				// Create the child process. 
				char programPath[MAX_PATH];
				if (!findFile((char*)program.c_str(), programPath))
					return -1;
				BOOL bSuccess = CreateProcessA(programPath, 
											(char*)cmd.c_str(),	// command line
											NULL,		// process security attributes
											NULL,		// primary thread security attributes
											TRUE,		// handles are inherited
											0,			// creation flags
											NULL,		// use parent's environment
											NULL,		// use parent's current directory
											&siStartInfo,	// STARTUPINFO pointer
											&piProcInfo);	// receives PROCESS_INFORMATION
				if (!bSuccess)
					return -1;
				WaitForSingleObject( piProcInfo.hProcess, INFINITE );
				_close_all();
				CloseHandle(piProcInfo.hProcess);
      			CloseHandle(piProcInfo.hThread);
				return 0;
			}

		public:
			process(string command) {
				// example "ls -l dir"
				istringstream iss(command);
				vector<string> args{istream_iterator<string>{iss},
                      istream_iterator<string>{}};
				if (args.size() > 1) _parse_command(args);
				program = args[0];
				arguments = args;
				arguments.erase(arguments.begin());
				_reset_handles_flags(); // default values
			}
			
			process(string path, vector<string> args) {
				// Example: ls, {"-l", "dir"}
				program = path;
				arguments = args;
				_reset_handles_flags(); // default values
			}

			process(vector<string> args) {
				// Example: {"ls", "-l", "dir"}
				program = args[0];
				arguments = args;
				arguments.erase(arguments.begin());
				if (arguments.size() > 0) _parse_command(args);
				_reset_handles_flags(); // default values
			}

			int set_forward_flag(string flag) {
				if (flag == ">" || flag == "1>")
					out_flag = ">";
				else if (flag == "2>")
					err_flag = "2>";
				else if (flag == "1>>" || flag == ">>")
					out_flag = ">>";
				else if (flag == "&>") {
					err_flag = "2>"; out_flag = ">";
				} else if (flag == "&>>") {
					err_flag = "2>>"; out_flag = ">>";
				} else
					return -1;
				return 0;
			}

			int forward_stdin(HANDLE in) {
				// handle provided
				if (in != GetStdHandle(STD_INPUT_HANDLE)) {
					DWORD lpdwFlags[100];
					if (!GetHandleInformation(in, lpdwFlags))
						return error_return("invalid handle");
					if ((lpdwFlags && GENERIC_READ) == 0)
						return error_return("bad handle flags");
					handle_in = in;
				}
				return 0;
			}

			int forward_stdin(const char* in) {
				// char* - file provided
				HANDLE handle = CreateFileA(in,
											GENERIC_READ, 
											0, 
											NULL, 
											CREATE_NEW, 
											FILE_ATTRIBUTE_READONLY, 
											NULL);
				if (handle == INVALID_HANDLE_VALUE)
					return error_return("can't CreateFile");
				handle_in = handle;
				return 0;
			}

			int forward_stdout(HANDLE out) {
				if (out != GetStdHandle(STD_OUTPUT_HANDLE)) {
					HANDLE handleout = _forward_outerr_helper(out, out_flag);
					if (handleout == INVALID_HANDLE_VALUE)
						return -1;
					handle_out = handleout;
				}
				return 0;
			}

			int forward_stderr(HANDLE out) {
				if (out != GetStdHandle(STD_ERROR_HANDLE)) {
					HANDLE handleout = _forward_outerr_helper(out, err_flag);
					if (handleout == INVALID_HANDLE_VALUE)
						return -1;
					handle_err = handleout;
				}
				return 0;
			}

			int forward_stdout(const char* out) {
				HANDLE handleout = _forward_open_helper((char*)out, out_flag);
				if (handleout == INVALID_HANDLE_VALUE)
					return -1;
				handle_out = handleout;
				return 0;
			}

			int forward_stderr(const char* out) {
				HANDLE handleout = _forward_open_helper((char*)out, err_flag);
				if (handleout == INVALID_HANDLE_VALUE)
					return -1;
				handle_err = handleout;
				return 0;
			}

			template <typename INPUT, typename OUTPUT, typename ERRPUT>
			int run_exec(INPUT in, OUTPUT out, ERRPUT err, bool reset=true) {
				result = forward_stdin(in);
				if (result < 0) return result;
				result = forward_stdout(out);
				if (result < 0) return result;
				result = forward_stderr(err);
				if (result < 0) return result;
				result = _execute(reset);
				return result;
			}

			int run_exec(bool reset=true) {
				result = _execute(reset);
				return result;
			}

			std::string grab_execute() {
				HANDLE read, write;
				std::string result = "";
				SECURITY_ATTRIBUTES saAttr;
				// Set the bInheritHandle flag so pipe handles are inherited.
				saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
				saAttr.bInheritHandle = TRUE;
				saAttr.lpSecurityDescriptor = NULL;
				if ( ! CreatePipe(&read, &write, &saAttr, 0) )
					return result;
				// Ensure the read handle to the pipe for STDOUT is not inherited.
				//if ( ! SetHandleInformation(read, HANDLE_FLAG_INHERIT, 0) )
				//	return result;
				if ( ! SetHandleInformation(write, HANDLE_FLAG_INHERIT, 0) )
					return result;
				if (_execute(true) < 0)
					return result;
				BOOL bSuccess;
				DWORD dwRead;
				CHAR chBuf[BUF_SIZE+1];
				while (true) {
					bSuccess = ReadFile(read, chBuf, BUF_SIZE, &dwRead, NULL);
					if (!bSuccess)
						return result;
					chBuf[dwRead] = '\0';
					result += std::string(chBuf);
					if (dwRead != BUF_SIZE)
						break;
				}
				return result;
			}
			
			std::string get_command() {
				std::string cmd = program;
				for (std::string arg : arguments)
					cmd += " "+arg;
				return cmd;
			}

			~process()
			{ _close_all(); /* close all handles */ }

			bool is_done() { return isexec; }
			int get_result() { return result; }
	};

	int cascade_execute(pipc::process p1, pipc::process p2) {
		/*
		made to execute commands like echo bin | cmd.exe /c dir
		grabs output of 1 process, passes it to the next
		*/
		SECURITY_ATTRIBUTES saAttr;
		// Set the bInheritHandle flag so pipe handles are inherited.
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		HANDLE pipe[2];
		// Create a pipe for the child process's STDOUT. 
		if ( ! CreatePipe(&pipe[0], &pipe[1], &saAttr, 0) )
			return error_return("Stdout CreatePipe");
		// Ensure the read handle to the pipe for STDOUT is not inherited.
		//if ( ! SetHandleInformation(pipes[i][0], HANDLE_FLAG_INHERIT, 0) )
		//	return error_return("Stdout SetHandleInformation");
		if ( ! SetHandleInformation(pipe[1], HANDLE_FLAG_INHERIT, 0) )
			return error_return("Stdout SetHandleInformation");
			
		DWORD dwRead;
		BOOL bSuccess;
		CHAR chBuf[BUF_SIZE];
		p1.forward_stdout(pipe[1]);
		p1.run_exec();
		
		bSuccess = ReadFile( pipe[0], chBuf, BUF_SIZE, &dwRead, NULL);
		if (!bSuccess)
			return -1;
		process p(p2.get_command()+" "+std::string(chBuf));
		p.run_exec();
		
		if (pipe[0] != INVALID_HANDLE_VALUE)
			CloseHandle(pipe[0]);
		if (pipe[1] != INVALID_HANDLE_VALUE)
			CloseHandle(pipe[1]);
		return 0;
	}
}
#endif