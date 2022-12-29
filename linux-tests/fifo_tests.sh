#!/bin/bash
option=$1

echo "option" $option

echo Creation tests:

fifo_create=./fifo_create
TESTS=(/tmp/fifo ./fifo /root/fifo)
EXPECTED=("0" "255" "255")

for i in ${!TESTS[@]}; do
	CASE=${TESTS[$i]}
	STRING="Test $i ${TESTS[$i]}"
	OUT=$($fifo_create $CASE)
	X=$?
	if [[ ${EXPECTED[$i]} -ne $X ]] # CASE timed out
	then echo $STRING - failure
	else echo $STRING - success
	fi
done;
echo

echo Communication tests:

TESTS=(fifo_test_fork_rw fifo_test_fork_wr
		fifo_test_rw fifo_test_wr
		spare_test_rw spare_test_wr)
NUMBERS=("0" "0" "0" "0" "" "")
HELLO="hello world!"
for i in ${!TESTS[@]}; do
	TEMPFILE="temp.txt"
	touch $TEMPFILE
	CASE=${TESTS[$i]}
	STRING="Test $i $CASE"
	timeout -k 0s 3s ./$CASE > $TEMPFILE 2> $TEMPFILE
	if [[ $? == 124 ]] # CASE timed out
	then echo $STRING timed out!
	else
		if [[ $(cat $TEMPFILE) == $HELLO ]] & [[ $? == 0 ]]
		then echo $STRING - success
		else 
			echo $STRING - failure
		fi
	fi
	rm $TEMPFILE
done;

echo

if [[ $option == "clean" ]]
then
for i in ${!TESTS[@]};
do
	rm ${TESTS[$i]}
done;
rm rw_child
rm wr_child
rm $fifo_create
fi


