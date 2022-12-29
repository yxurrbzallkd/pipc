## PIPC Windows

## Process
```c++
namespace pipc {
	class process {
		private:
			string program;
			vector<string> arguments;
			LPWSTR command;
			vector<string>::iterator it; // for reuse
			std::pair<string, string> wh; // for reuse
			PROCESS_INFORMATION piProcInfo;
			STARTUPINFO siStartInfo;
			HANDLE handle_in, handle_out, handle_err;
			string out_flag, err_flag;
			bool isexec = false;
			int result;
			char buf[BUF_SIZE+1];

			bool _findFile(CHAR* startDirectory, CHAR* fileToFind, CHAR* foundFile, bool recurse)
			// looks for file in given dir
			bool findFileCWD(CHAR* fileToFind, CHAR* foundFile) 
			// looks for file in cwd
			bool findFile(CHAR* fileToFind, CHAR* foundFile) {
			// looks for file in cwd and in GetWindowsDirectory(), and system32 inside it

			int _set_all() {
				// set inheritable with SetHandleInformation
				// duplicate handles
				// set handles in child startup structures
			}
			void _close_all()
				// close child stdin\out\err handles
			int _execute(bool reset)

		public:
			process(string command)
				// Example "ls -l dir"
			process(string path, vector<string> args)
				// Example: ls, {"-l", "dir"}
			process(vector<string> args)
				// Example: {"ls", "-l", "dir"}

			int set_forward_flag(string flag)
			int forward_stdin(HANDLE in)
			int forward_stdin(const char* in)
				// const char* - file provided
			int forward_stdout(HANDLE out)
			int forward_stderr(HANDLE out)
			int forward_stdout(const char* out)
			// const char* file provided
			int forward_stderr(const char* out)

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

			std::string grab_execute()
			std::string get_command()
			~process()
			{ _close_all(); /* close all handles */ }

			bool is_done() { return isexec; }
			int get_result() { return result; }
	};
```

## Conveyor
```c++
	int cascade_execute(pipc::process p1, pipc::process p2) {
		/*
		made to execute commands like echo bin | cmd.exe /c dir
		grabs output of 1 process, passes it to the next
		*/
		// ,,,
		// Set the bInheritHandle flag so pipe handles are inherited.
		// ...
		// Create a pipe for the child process's STDOUT. 
		// Ensure the read handle to the pipe for STDOUT is not inherited.
		// read from one process, append command to another
}
```

## Named pipe
```c++
namespace pipc {
	class fifo {
		private:
			CHAR* fifo_name;
			DWORD fifo_mode;
			BOOL fifo_create;
			HANDLE fifo_handle;
			BOOL issetup = false;

		public:
			fifo(CHAR* name) : fifo_name(name) {
				fifo_create = true;
			}
			fifo(CHAR* name, DWORD openMode) : fifo_name(name), fifo_mode(openMode) {
				fifo_create = false;
			}
			int setup() {
				if (fifo_create) {
					fifo_handle = CreateNamedPipeA(...)
				} else {
					fifo_handle = CreateFileA(...)
			}

			int read_fifo(CHAR* buf, size_t n, DWORD& dwRead) {
				// ...
				// if (fifo_create) - ConnectNamedPipe(fifo_handle, NULL)
				ReadFile(fifo_handle, buf, n, &dwRead, NULL)
				// if (fifo_create) - DisconnectNamedPipe(fifo_handle);
				// ...
			}

			int write_fifo(CHAR* buf, size_t n,  DWORD& dwWritten) {
				// ...
				// if (fifo_create) - ConnectNamedPipe(fifo_handle, NULL)
				WriteFile(fifo_handle, buf, n, &dwWritten, NULL)
				// if (fifo_create) - DisconnectNamedPipe(fifo_handle);
				// ...
			}

			~fifo() {
				if (fifo_create)
					DisconnectNamedPipe(fifo_handle);
				else
					CloseHandle(fifo_handle);
			}
	};
}
```

## Shmem
```c++
namespace pipc {
	class shmem {
		private:
			CHAR* shm_name;
			DWORD shm_flag;
			BOOL shm_create;
			size_t shm_size;
			HANDLE shm_handle;
			CHAR* map;
			BOOL issetup = false;

		public:
			shmem(CHAR* name, size_t size, BOOL create, DWORD openMode)
			: shm_name(name), shm_size(size), shm_create(create), shm_flag(openMode)
			{ }
			int setup() {
				if (shm_create) {
					shm_handle = CreateFileMappingA( ... )
				} else {
					shm_handle = OpenFileMappingA( ... )
				    if (shm_handle == NULL)
						return SHM_ERROR | FAILED_TO_OPEN_MAPPING;
				}
				map = (CHAR*) MapViewOfFile( ... )
				// ...
			int read_shm(CHAR* buf, size_t n) {
				// CopyMemory(buf, map, n);
			int write_shm(CHAR* buf, size_t n)
				// CopyMemory(map, buf, n);
			~shmem() {
				if (shm_create)
					DisconnectNamedPipe(shm_handle);
				else
					CloseHandle(shm_handle);
			}
	};
}
```

