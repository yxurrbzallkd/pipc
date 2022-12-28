#ifndef PIPE_HEADER
#define PIPE_HEADER
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "process_helpers.hpp"
#include <pipc/errors_and_warnings.hpp>

namespace pipc {
	int pipe_execute(std::vector<pipc::process> ps) {
		if (ps.size() < 2)
			return ARGUMENT_ERROR;

		int n = ps.size();
		int pipes[n-1][2];
		for (int i = 0; i < n-1; i++)
			if (pipe(pipes[i]) < 0)
				return PIPE_ERROR | FAILED_TO_PIPE;

		ps[0].forward_stdout(pipes[0][1]);
		if (ps[0].run_exec() < 0)
			return PROCESS_ERROR | PROCESS_FAILED;
		for (int i = 1; i < n-1; i++) {
			close(pipes[i-1][1]);
			ps[i].forward_stdin(pipes[i-1][0]);
			ps[i].forward_stdout(pipes[i][1]);
			if (ps[i].run_exec() < 0)
				return PROCESS_ERROR | PROCESS_FAILED;
			close(pipes[i-1][0]);
		}
		// last process - don't forward stdout
		close(pipes[n-2][1]);
		ps[n-1].forward_stdin(pipes[n-2][0]);
		if (ps[n-1].run_exec() < 0)
			return PROCESS_ERROR | PROCESS_FAILED;
		close(pipes[n-2][0]);
		return n;
	}

	int pipe_execute(std::initializer_list<process> processes) {
		std::vector<pipc::process> ps(processes);
		return pipe_execute(ps);
	}
}
#endif