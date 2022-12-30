#!/bin/bash
option=$1
echo "option" $option

basic_process_test="./basic_process_test"
piped_process_test="./piped_process_test"
file_forward_test="./file_forward_test"
hello="./hello"

echo Hello test:
if [[ $($hello) == $(echo "Hello World!") ]]
then echo success
else echo failure
fi
echo

echo Forwarding tests:

FILE="file.txt"
#!!! Problem: bash probably steals the forwarding from the CASE
file_forward_test="file_forward_test"
CASE="echo hello world"
FULL_OUT=""
FORWARD_TESTS=(">" ">>");
for i in ${!FORWARD_TESTS[@]}; do
	flag=${FORWARD_TESTS[$i]}
	OUT=$($CASE)
	OUT_P=$(./$file_forward_test $CASE "$flag" $FILE)
	STRING="$CASE $flag $FILE"
	FULL_OUT=$FULL_OUT$OUT
	if [[ $(cat $FILE) == $FULL_OUT ]] && [[ $? == "0" ]]
	then echo $STRING - success
	else 
		echo $STRING - failure
	fi
	FULL_OUT="$FULL_OUT"$'\n'
done;
rm $FILE

echo

echo Basic tests:

TESTS=("echo hello world" "ls" "ls -l");
for i in ${!TESTS[@]}; do
	CASE=${TESTS[$i]}
	STRING="Test $i : $CASE"
	if [[ $(./$basic_process_test $CASE) == $($CASE) ]] && [[ $? == "0" ]]
	then echo $STRING - success
	else 
		echo $STRING - failure
	fi
done;

echo


echo Piped process tests:

mkdir "test"
cd "test"
touch hello.txt
echo hello world > hello.txt
touch redme.md
echo hello Diana > name_hello.txt
cd ..
if [[ $($piped_process_test ls "test" - grep .txt - wc) == $(ls \test | grep .txt | wc) ]] && [[ $? == "0" ]]; then
	echo "ls \test | grep .txt | wc" - success
else
	echo "ls \test | grep .txt | wc" - failure
fi

if [[ $($piped_process_test cat test/hello.txt - wc) == $(cat test/hello.txt | wc) ]] && [[ $? == "0" ]]; then
	echo "cat hello.txt | wc" - success
else
	echo "cat hello.txt | wc" - failure
fi
echo

echo Run-grab test:
run_grab_test="./run_grab_test"
if [[ $($run_grab_test) == $(echo -e "hello world\n") ]] && [[ $? == "0" ]];
then echo success
else echo failure
fi
echo

rm -r "test"

if [[ $option == "clean" ]]
then
rm $file_forward_test
rm $basic_process_test
rm $piped_process_test
rm $run_grab_test
fi
