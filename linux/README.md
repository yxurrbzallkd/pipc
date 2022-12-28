# PIPC Linux

## Process
```
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
```

## Pipes


## Fifo


## Shmem

