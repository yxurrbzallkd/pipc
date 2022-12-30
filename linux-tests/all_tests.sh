#!/bin/bash
option=""
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
mkdir bin
cd bin
cmake .. -G"Unix Makefiles"
make
cd ..
fi

option=$1
echo "option" $option

echo Testing PIPC library:

./process_tests.sh $option
./fifo_tests.sh $option
./shmem_tests.sh $option
./socket_tests.sh $option

if [[ $option == "clean" ]]
then
rm hello
rm infinite
rm background_test
rm -r bin
fi
