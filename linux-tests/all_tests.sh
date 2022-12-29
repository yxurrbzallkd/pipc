!/bin/bash
echo Compiling tests:
#mkdir bin
#cd bin
#cmake ..
#make
#cd ..
#echo

option=$1
echo "option" $option

echo Testing PIPC library:

./process_tests.sh $option
./fifo_tests.sh $option
./shmem_tests.sh $option

if [[ $option == "clean" ]]
then
rm hello
rm -r bin
fi
