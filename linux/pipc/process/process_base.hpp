#ifndef BASIC_PROCESS_HEADER
#define BASIC_PROCESS_HEADER
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
#include <pipc/errors_and_warnings.hpp>

#define BUF_SIZE 1024

using namespace std;

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

			void _reset_fds_flags() {
				// restore default values
				fd_in = STDIN_FILENO;
				fd_out = STDOUT_FILENO;
				fd_err = STDERR_FILENO;
				out_flag = ">", err_flag = "2>";
			}

			void _parse_command(vector<string> args) {
				/*
				looks for forwarding symbols and respective files
				cuts them out, forwards stdin\out\err
				*/
				pair<string, string> wh;
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

			int _dup_all() {
				/* replaces stdin\out\err with forwarding fd's */
				if (fd_in != STDIN_FILENO)
					if (dup2(fd_in, STDIN_FILENO) < 0)
						return FORWARD_ERROR | FAILED_TO_DUP;
				if (fd_out != STDOUT_FILENO)
					if (dup2(fd_out, STDOUT_FILENO) < 0)
 						return FORWARD_ERROR | FAILED_TO_DUP;
				if (fd_err != STDERR_FILENO)
					if (dup2(fd_err, STDERR_FILENO) < 0)
						return FORWARD_ERROR | FAILED_TO_DUP;
				return SUCCESS;
			}

			void _close_all() {
				// closes fd's if not standard used
				if (fd_in != STDIN_FILENO)
					close(fd_in);
				if (fd_out != STDOUT_FILENO)
					close(fd_out);
				if (fd_err != STDERR_FILENO)
					close(fd_err);
			}

			string reader(int fd) {
				// reads from fd and combines into string
				string output = "";
				int r = BUF_SIZE;
				while (r == BUF_SIZE) {
					r = read(fd, buf, BUF_SIZE);
					if (r < 0) {
						warn("failed to read");
						return "";
					}
					buf[r] = '\0';
					output += string(buf);
				}
				return output;
			}

		public:
			basic_process(string command) {
				// example "ls -l dir"
				istringstream iss(command);
				vector<string> args{istream_iterator<string>{iss},
                      istream_iterator<string>{}};
				if (args.size() > 1) _parse_command(args);
				program = args[0];
				arguments = args;
				_reset_fds_flags(); // default values
			}
			
			basic_process(string path, vector<string> args) {
				// Example: ls, {"-l", "dir"}
				program = path;
				arguments = args;
				arguments.insert(arguments.begin(), program);
				_reset_fds_flags(); // default values
			}

			basic_process(vector<string> args) {
				// Example: {"ls", "-l", "dir"}
				program = args[0];
				arguments = args;
				if (args.size() > 1) _parse_command(args);
				_reset_fds_flags(); // default values
			}

			int set_forward_flag(string flag) {
				if (flag == ">" || flag == "1>")
					out_flag = ">";
				else if (flag == "2>")
					err_flag = "2>";
				else if (flag == ">>" || flag == "1>>")
					out_flag = ">>";
				else if (flag == "&>") {
					err_flag = "2>"; out_flag = ">";
				} else if (flag == "&>>") {
					err_flag = "2>>"; out_flag = ">>";
				} else
					return -1;
				return SUCCESS;
			}

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

			int forward_stdin(const char* in) {
				// char* - file provided
				int fd = open(in, R_OK);
				if (fd < 0) return FAILED_TO_OPEN;
				fd_in = fd;
				return SUCCESS;
			}

			int forward_stdout(int out) {
				// provide fd
				if (out != STDOUT_FILENO) {
					int fdout = _forward_outerr_helper(out, out_flag);
					if (fdout < 0)
						return PROCESS_ERROR | FORWARD_ERROR;
					fd_out = fdout;
				}
				return SUCCESS;
			}

			int forward_stderr(int out) {
				// provide fd
				if (out != STDERR_FILENO) {
					int fdout = _forward_outerr_helper(out, err_flag);
					if (fdout < 0)
						return PROCESS_ERROR | FORWARD_ERROR;
					fd_err = fdout;
				}
				return SUCCESS;
			}

			int forward_stdout(const char* out) {
				// provide filename
				int fdout = _forward_open_helper(out, out_flag);
				if (fdout < 0)
					return fdout;
				fd_out = fdout;
				return SUCCESS;
			}

			int forward_stderr(const char* out) {
				// provide filename
				int fdout = _forward_open_helper(out, err_flag);
				if (fdout < 0)
					return fdout;
				fd_err = fdout;
				return SUCCESS;
			}

			std::vector<string> get_args() {
				// return argument
				return arguments;
			}

			int get_result() {
				// return result of execution
				if (!isexec)
					return PROCESS_ERROR;
				return result;
			}

			std::string get_command() {
				// returns command
				std::string command = "";
				for (std::string s : arguments)
					command += s + " ";
				return command;
			}
			
			~basic_process()
			{ _close_all(); /* close all fds */ }
	};
}

#endif