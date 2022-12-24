TESTS=(fifo_test_rw fifo_test_wr fifo_test_wait_rw fifo_test_wait_wr fifo_test_fork_rw fifo_test_fork_wr)
HELLO="hello world!"
for i in ${!TESTS[@]}; do
	TEMPFILE="temp.txt"
	touch $TEMPFILE
	COMMAND=${TESTS[$i]}
	STRING="Test $i : $COMMAND"
	timeout 3 ./$COMMAND 0 "1>" $TEMPFILE "2>" $TEMPFILE
	if [[ $? == 124 ]] # command timed out
	then echo $STRING timed out!
	else
		OUT=$(./$COMMAND 0)
		echo "out" $OUT
		if [[ $OUT == $HELLO ]]
		then echo $STRING - success
		else 
			echo $STRING - failure
		fi
	fi
done;
