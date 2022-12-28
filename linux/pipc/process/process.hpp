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
#include <pipc/errors_and_warnings.hpp>

#define BUF_SIZE 1024

using namespace std;

namespace pipc {
	class process {
		private:
			string program;
			vector<string> arguments;
			vector<string>::iterator it; // for reuse
			std::pair<string, string> wh; // for reuse
			int fd_in, fd_out, fd_err;
			string out_flag, err_flag;
			bool isexec = false;
			int result;
			char buf[BUF_SIZE+1];

			void _reset_fds_flags() {
				fd_in = STDIN_FILENO;
				fd_out = STDOUT_FILENO;
				fd_err = STDERR_FILENO;
				out_flag = ">", err_flag = "2>";
			}

			bool _forward_parse(string s,
								vector<string>& args) {
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

			void _parse_command(vector<string> args) {
				for (string s : {">", "1>", ">>", "1>>"})
					if (_forward_parse(s, args))
						forward_stdout(wh.second.c_str());
				for (string s : {"2>", "2>>"})
					if (_forward_parse(s, args))
						forward_stderr(wh.second.c_str());
				if (_forward_parse("&>", args)) {
					out_flag = "1>"; err_flag = "2>";
					forward_stdout(wh.second.c_str());
					forward_stderr(wh.second.c_str());
				}
				if (_forward_parse("&>>", args)) {
					out_flag = "1>>"; err_flag = "2>>";
					forward_stdout(wh.second.c_str());
					forward_stderr(wh.second.c_str());
				}
			}

			int _dup_all() {
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
				if (fd_in != STDIN_FILENO)
					close(fd_in);
				if (fd_out != STDOUT_FILENO)
					close(fd_out);
				if (fd_err != STDERR_FILENO)
					close(fd_err);
			}

			int _grab_execute(std::pair<string, string>& res) {
				isexec = true;
				int r;
				int pipefd_out[2];
				int pipefd_err[2];
				if ((pipe(pipefd_out) < 0) || (pipe(pipefd_err) < 0))
					return PIPE_ERROR | FAILED_TO_PIPE;
				pid_t pid = fork();
				if (pid < 0)
					return PROCESS_ERROR | FAILED_TO_FORK;
				if (pid == 0) {
					if ((close(pipefd_out[0]) < 0) || (close(pipefd_err[0]) < 0))
						return PIPE_ERROR | FAILED_TO_CLOSE;
					if ((dup2(pipefd_out[1], STDOUT_FILENO) < 0) || (dup2(pipefd_err[1], STDERR_FILENO) < 0))
						return PIPE_ERROR | FAILED_TO_DUP;
					r = execvp(program.c_str(), vector_to_chararr(arguments));
					if ((close(pipefd_out[1]) < 0) || (close(pipefd_err[1]) < 0))
						return PIPE_ERROR | FAILED_TO_CLOSE;
					exit(r);
				} else {
					if ((close(pipefd_out[1]) < 0) || (close(pipefd_err[1]) < 0))
						return PIPE_ERROR | FAILED_TO_CLOSE;
					waitpid(pid, &r, 0);
					res.first = reader(pipefd_out[0]);
					res.second = reader(pipefd_err[0]);
					if ((close(pipefd_out[0]) < 0) || (close(pipefd_err[0]) < 0))
						return PIPE_ERROR | FAILED_TO_CLOSE;
				}
				_close_all();
				_reset_fds_flags();
				return r;
			}

			int _execute() {
				int res;
				pid_t pid = fork();
				if (pid < 0)
					return PROCESS_ERROR | FAILED_TO_FORK;
				else if (pid == 0) {
					if (_dup_all() != SUCCESS)
						return PROCESS_ERROR | FORWARD_ERROR | FAILED_TO_DUP;
					execvp(program.c_str(), vector_to_chararr(arguments));
				} else {
					waitpid(pid, &res, 0);
				}
				_close_all();
				_reset_fds_flags();
				isexec = true;
				return res;
			}

			string reader(int fd) {
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
			process(string command) {
				// example "ls -l dir"
				istringstream iss(command);
				vector<string> args{istream_iterator<string>{iss},
                      istream_iterator<string>{}};
				if (args.size() > 1) _parse_command(args);
				program = args[0];
				arguments = args;
				_reset_fds_flags(); // default values
			}
			
			process(string path, vector<string> args) {
				// Example: ls, {"-l", "dir"}
				program = path;
				arguments = args;
				arguments.insert(arguments.begin(), program);
				_reset_fds_flags(); // default values
			}

			process(vector<string> args) {
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
				if (fd < 0) return FILE_ERROR | FAILED_TO_OPEN;
				fd_in = fd;
				return SUCCESS;
			}

			int forward_stdout(int out) {
				if (out != STDOUT_FILENO) {
					int fdout = _forward_outerr_helper(out, out_flag);
					if (fdout < 0)
						return PROCESS_ERROR | FORWARD_ERROR;
					fd_out = fdout;
				}
				return SUCCESS;
			}

			int forward_stderr(int out) {
				if (out != STDERR_FILENO) {
					int fdout = _forward_outerr_helper(out, err_flag);
					if (fdout < 0)
						return PROCESS_ERROR | FORWARD_ERROR;
					fd_err = fdout;
				}
				return SUCCESS;
			}

			int forward_stdout(const char* out) {
				int fdout = _forward_open_helper(out, out_flag);
				if (fdout < 0)
					return fdout;
				fd_out = fdout;
				return SUCCESS;
			}

			int forward_stderr(const char* out) {
				int fdout = _forward_open_helper(out, err_flag);
				if (fdout < 0)
					return fdout;
				fd_err = fdout;
				return SUCCESS;
			}

			template <typename IN, typename OUT, typename ERR>
			int run_exec(IN in, OUT out, ERR err) {
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
				result = _execute();
				return result;
			}

			std::pair<string, string> run_grab() {
				std::pair<string, string> res;
				result = _grab_execute(res);
				if (result < 0)
					warn("invalid result");
				return res;
			}

			std::pair<string, string> run_grab(int in) {
				std::pair<string, string> res;
				if (forward_stdin(in) != SUCCESS)
					return res;
				result = _grab_execute(res);
				if (result < 0)
					warn("invalid result");
				return res;
			}

			std::vector<string> get_args()
			{ return arguments; }

			~process()
			{ _close_all(); /* close all fds */ }

			bool is_done() { return isexec; }
			int get_result() { return result; }

			std::string get_command() {
				std::string command = "";
				for (std::string s : arguments)
					command += s + " ";
				return command;
			}
	};
}

#endif