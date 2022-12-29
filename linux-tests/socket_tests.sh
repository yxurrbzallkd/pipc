#!/bin/bash

option=$1

test="socket_basic_test"

echo Socket tests:

if [[ $(./$test) == $(echo hello world) ]]
then
echo $test - success
else
echo $test - failure
fi
echo

if [[ $option == "clean" ]]
then
rm $test
fi
