!/bin/bash
echo Compiling tests:
mkdir bin
cd bin
cmake ..
make
cd ..
echo
echo Testing PIPC library:

./process_tests.sh
./fifo_tests.sh
./shmem_tests.sh

rm hello
rm -r bin