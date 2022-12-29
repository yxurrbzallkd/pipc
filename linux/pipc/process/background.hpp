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
#include "basic_process.hpp"
#include <pipc/errors_and_warnings.hpp>

#define BUF_SIZE 1024

using namespace std;

namespace pipc {
	class background_process : public basic_process {
		private:
			bool islaunched = false;
			pid_t pid;

			int _launch() {
				pid = fork();
				if (pid < 0)
					return PROCESS_ERROR | FAILED_TO_FORK;
				else if (pid == 0) {
					if (_dup_all() != SUCCESS)
						return PROCESS_ERROR | FORWARD_ERROR | FAILED_TO_DUP;
					execvp(program.c_str(), vector_to_chararr(arguments));
				}
				islaunched = true;
				return SUCCESS:
			}

			template <typename IN, typename OUT, typename ERR>
			int launch(IN in, OUT out, ERR err) {
				result = forward_stdin(in);
				if (result < 0) return result;
				result = forward_stdout(out);
				if (result < 0) return result;
				result = forward_stderr(err);
				if (result < 0) return result;
				return _launch();;
			}

			int launch() {
				return _execute();
			}

			int wait_for() {
				waitpid(pid, &result, 0);
				return result;
			}

			int is_finished()
			{ return isfinished; }

			int is_launched()
			{ return islaunched; }
	};
}

#endif