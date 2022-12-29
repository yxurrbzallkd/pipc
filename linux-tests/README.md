# How to Test

```./all_test.sh [clean]```

if run with clean option - deletes all executables

# Useful commands that can be used for testing

- ```netstat -ntpl```
- ls /proc
- kill -9 <pid>

# Tests

automated - expect to see success

## hello
prints "Hello World!"

## basic_process_test
```./basic_process_test echo hello``` == ```echo hello```

## file_forward_test
```./file_forward_test <arg1> <arg2> ... <argN> <forwarding flag (>, >> etc.)> <filename>```

## piped_process_test
```./piped_process_test ls dir - grep .cpp``` == ```ls dir | grep .cpp```

## run_grab_test
"Grabs" the output of ```echo hello world```
and prints it. Does not accept arguments

## fifo_create
Creates fifo with a given name. Expect it to fail with invalid names.

## fifo_test_(rw\wr)
Usage: ```./fifo_test_(rw\r) <anything>``` - if no arguments provided, runs the parent - creates fifo, then launches child - the same program with pipe name as argument = child mode and writes in child - reads in parent \ writes in parent - reads in child. Expect to see hello world

## fifo_test_fork_(rw\wr)
Same as before, but with ```fork()``` instead of ```execlp()```

## spare_test_(rw\wr)
Same as before, but child is in entirely different file - **(rw\wr)_child**

## shmem_test_single
Creates shm and writes to it and reads to it from the same process.

## shmem_test_(rw\wr)
Creates a shm, launches a child - the same program with shm name as argument. parent writes - child reads \ child writes - parent reads.

## socket_basic_test
Creates a socket, ```fork()```s, launches the server in parent with processing function that returns hello message to every child, child exchanges messages with parent and prints what it received. Expect to see hello world.

