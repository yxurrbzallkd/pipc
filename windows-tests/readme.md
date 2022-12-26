# Tests description

## basic_process_test
connects argv pieces into a command and runs it

## file_forward_test
forwards hello world into a given file, provide filename flag in this order

## conveyor_test
runs a conveyor - not a pip connection, but grabs output of process 1 into a string and passes it to process 2
runs echo bin | ls

## shm_basic_test
opens a shm, writes than reads into it, 1 process no sharing

## shm_share_test
run with one extra argument - any argument

## fifo_test
run with one extra argument - any argument
