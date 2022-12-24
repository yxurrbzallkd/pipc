basic_process_test="./basic_process_test"
piped_process_test="./piped_process_test"
file_forward_test="./file_forward_test"
hello="./hello"
test_programs=(basic_process_test piped_process_test hello)

echo Hello test:
if [[ $($hello) == $(echo "Hello World!") ]]
then echo success
else echo failure
fi
echo

echo Basic tests:

TESTS=("echo hello world" "ls" "ls -l");
for i in ${!TESTS[@]}; do
	COMMAND=${TESTS[$i]}
	STRING="Test $i : $COMMAND"
	if [[ $(./$basic_process_test $COMMAND) == $($COMMAND) ]]
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
if [[ $($piped_process_test ls "test" - grep .txt - wc) == $(ls \test | grep .txt | wc) ]]; then
	echo "ls \test | grep .txt | wc" - success
else
	echo "ls \test | grep .txt | wc" - failure
fi

if [[ $($piped_process_test cat test/hello.txt - wc) == $(cat test/hello.txt | wc) ]]; then
	echo "cat hello.txt | wc" - success
else
	echo "cat hello.txt | wc" - failure
fi
echo

echo Forwarding tests:

FILE="./test/file.txt"
CMD_FILE="./test/cmdfile.txt"

#!!! Problem: bash probably steals the forwarding from the command

COMMAND="echo hello world"
FORWARD_TESTS=(">" ">>");
for i in ${!FORWARD_TESTS[@]}; do
	flag=${FORWARD_TESTS[$i]}
	OUT_P=$(./$file_forward_test $COMMAND "$flag" $FILE)
	STRING="$COMMAND $flag $FILE"
	if [[ $(cat $FILE) != "hello world" ]]
	then echo $STRING - failure
	else 
		echo $STRING - success
	fi
done;

echo

#rm -r "test"

#rm $test_programs
