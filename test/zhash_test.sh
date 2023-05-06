#!/bin/bash

gcc -Wall -Wextra -g ../src/zhash.c ./zhash_test.c -o hash

./hash
if [ $? -ne 0 ]; then
  echo '[FAIL]: At least one test failed'
  exit 1
fi

# check for memory leaks on Linux
if command -v valgrind > /dev/null; then
  valgrind --quiet --leak-check=yes ./hash
  if [ $? -ne 0 ]; then
    echo '[FAIL]: Memory leak found'
    exit 1
  fi
# check for memory leaks on macOS
elif command -v leaks > /dev/null; then
  tempfile=mktemp
  (MallocStackLogging=1 leaks --quiet --atExit -- ./hash) >$tempfile 2>&1
  if [ $? -ne 0 ]; then
    cat $tempfile
    rm $tempfile
    echo '[FAIL]: Memory leak found'
    exit 1
  fi
  rm $tempfile
else
  echo '[ERROR]: No memory checker found'
  exit 1
fi

rm hash
