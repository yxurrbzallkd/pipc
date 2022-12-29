#/usr/hello/bash
option=$1 # if clean - delete exes

#mkdir hello
#cd hello
#cmake .. -G"Unix Makefiles"
#make
#cd ..

echo Running tests:

echo doing hello
./hello.exe
echo

echo "doing echo hello from cmd"
./basic_process_test.exe "cmd.exe" "\\/c" "echo" hello
echo

echo "doing echo hello from powershell"
./basic_process_test.exe "powershell.exe" "echo" hello
echo

mkdir hello
echo hello > ./hello/file.txt
./conveyor_test.exe
echo

#!! doesn't work
echo "doing dir hello" 
./basic_process_test.exe "cmd.exe" "\\/c" "dir" "hello"
echo

echo doing ls hello
./basic_process_test.exe "powershell.exe" "ls" "hello"
echo

echo hello > file.txt
echo doing cat a file with hello
./basic_process_test.exe "powershell.exe" "cat" "file.txt"
rm file.txt
echo

echo Basic shm test - expect to see read hello world and wrote hello world:
./shm_basic_test.exe
echo

echo Complicated shm test expect hello:
./shm_share_test.exe 0
echo

echo Basic fifo test hello world:
./fifo_test.exe hello
echo

rm -r hello

if [[ option == "clean" ]]
then
rm fifo_test.exe
rm shm_basic_test.exe
rm shm_share_test.exe
rm basic_process_test.exe
rm conveyor_test.exe
rm hello.exe
rm fifo_test.exe
fi
