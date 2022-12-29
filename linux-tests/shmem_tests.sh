#!/bin/bash
option=$1
echo "option" $option

echo Tests:

HELLO="hello world!"
TESTS=(shmem_test_single shmem_test_rw shmem_test_wr)
for i in ${!TESTS[@]}; do
	CASE=${TESTS[$i]}
	STRING="Test $i : $CASE"
	if [[ $(sudo ./$CASE) == $HELLO ]]
	then echo $STRING - success
	else 
		echo $STRING - failure
	fi
done;
echo

if [[ $option == "clean" ]]
then
for i in ${!TESTS[@]};
do
rm ${TESTS[$i]}
done
fi
