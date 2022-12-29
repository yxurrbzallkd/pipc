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
#include "process_base.hpp"
#include <pipc/errors_and_warnings.hpp>

#define BUF_SIZE 1024

using namespace std;

namespace pipc {
	class process : public basic_process {
		private:
			int _grab_execute(std::pair<string, string>& res) {
				/*
				creates 2 pipes for stdout and stderr
				forks
				dup2's child's stdout\err to reading ends of pipes
				reads from pipes in parent
				stores read from stdout and stderr in res.first and res.second respectively
				*/
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
				/*
				forks
				dup2's stdin/out/err
				calls execvp
				waits in parent
				returns error code
				*/
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

		public:
			using basic_process::basic_process;

			template <typename IN, typename OUT, typename ERR>
			int run_exec(IN in, OUT out, ERR err) {
				/*
				run the proces
				accepts fd's and filenames
				pass standard if don't want to forward
				*/
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
				// without arguments
				result = _execute();
				return result;
			}

			std::pair<string, string> run_grab() {
				/*
				grabs stdout and stderr
				stores stdout in pair's first, stderr in second
				*/
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
	};
}

#endif