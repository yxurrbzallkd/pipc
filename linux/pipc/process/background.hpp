#ifndef BACKGROUND_HEADER
#define BACKGROUND_HEADER
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
	class background_process : public basic_process {
		private:
			bool isrunning = false;
			pid_t pid;

			int _launch() {
				pid = fork();
				if (pid < 0)
					return PROCESS_ERROR | FAILED_TO_FORK;
				if (pid == 0) {
					if (_dup_all() != SUCCESS)
						return PROCESS_ERROR | FORWARD_ERROR | FAILED_TO_DUP;
					execvp(program.c_str(), vector_to_chararr(arguments));
					exit(result);
				} else {
					isrunning = true;
					return SUCCESS;
				}
				exit(0);
			}

		public:
			using basic_process::basic_process;
			template <typename IN, typename OUT, typename ERR>
			int launch(IN in, OUT out, ERR err) {
				isexec = false;
				result = forward_stdin(in);
				if (result < 0) return result;
				result = forward_stdout(out);
				if (result < 0) return result;
				result = forward_stderr(err);
				if (result < 0) return result;
				return _launch();;
			}

			int launch() {
				isexec = false;
				return _launch();
			}

			int wait_for() {
				if (isexec)
					return result;
				if (!isrunning)
					return SUCCESS;
				waitpid(pid, &result, 0);
				isrunning = false;
				isexec = true;
				return result;
			}

			bool is_finished() {
				if (isexec)
					return true;
				if (isrunning) {
					if (kill(pid, 0) == 0)
						return false;
					if (errno != ESRCH)
						return false;
					isrunning = false;
					isexec = true;
				}
				return isexec;
			}

			bool is_launched() {
				return isrunning;
			}

			bool is_running() {
				if (kill(pid, 0) < 0)
					return false;
				return true; 
			}

			bool kill_it() {
				kill(pid, SIGKILL);
				if (kill(pid, SIGKILL) < 0)
					return false;
				isexec = false;
				isrunning = false;
				return true;
			}
	};
}

#endif