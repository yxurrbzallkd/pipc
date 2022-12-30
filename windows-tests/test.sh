#/usr/hello/bash
option="" # if clean - delete exes
compile="no"
for var in "$@"
do
if [[ $var == "clean" ]]
then
option=$var
fi
if [[ $var == "compile" ]]
then
compile=$var
fi
done

if [[ $compile == "compile" ]]
then
echo Compiling
mkdir hello
cd hello
cmake .. -G"Unix Makefiles"
make
cd ..
fi

echo Running tests:

echo Hello test:
OUT=$(./hello.exe)
if [[ $OUT == "hello" ]] && [[ $? == 0 ]]
then
echo "success"
else
echo "failure"
fi
echo

echo "Doing echo hello from cmd:"
OUT=$(./basic_process_test.exe "cmd.exe" "\\/c" "echo" hello)
if [[ $OUT == "hello" ]] && [[ $? == 0 ]]
then
echo "success"
else
echo "failure"
fi
echo

echo "Doing echo hello from powershell:"
OUT=$(./basic_process_test.exe "powershell.exe" "echo" hello)
if [[ $OUT == "hello" ]] && [[ $? == 0 ]]
then
echo "success"
else
echo "failure"
fi
echo


echo "conveyor test echo ../ | dir"
./conveyor_test.exe
echo

#!! doesn't work
echo "doing dir ../../" 
OUT=$(./basic_process_test.exe "cmd.exe" "\\/c" "dir" "..\\..\\")
echo

echo "Creating a test directory hello:"
mkdir hello
echo hello > ./hello/file.txt

echo "Doing ls hello:"
./basic_process_test.exe "powershell.exe" "ls" "hello"
echo

echo "Doing cat a file with hello:"
OUT=$(./basic_process_test.exe "powershell.exe" "cat" "hello/file.txt")
if [[ $OUT == "hello" ]] && [[ $? == 0 ]]
then
echo "success"
else
echo "failure"
fi
echo

echo "Basic shm test - expect to see read hello world and wrote hello world:"
./shm_basic_test.exe
echo

echo "Complicated shm test expect hello:"
./shm_share_test.exe 0
echo

echo "Basic fifo test hello world:"
./fifo_test.exe hello
echo

rm -r hello

if [[ $option == "clean" ]]
then
rm ./fifo_test.exe
rm ./shm_basic_test.exe
rm ./shm_share_test.exe
rm ./basic_process_test.exe
rm ./conveyor_test.exe
rm ./hello.exe
rm ./fifo_test.exe
rm ./file_forward_test.exe
rm -r bin
fi
