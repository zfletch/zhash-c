#!/bin/bash

function run_tests() {
  input_files=$1
  output_file=$2

  echo "[INFO]: Testing $output_file"

  gcc -Wall -Wextra -g $input_files -o $output_file

  ./$output_file
  if [ $? -ne 0 ]; then
    echo '[FAIL]: At least one test failed'
    exit 1
  fi

  # check for memory leaks on Linux
  if command -v valgrind > /dev/null; then
    valgrind --quiet --leak-check=yes ./$output_file
    if [ $? -ne 0 ]; then
      echo '[FAIL]: Memory leak found'
      exit 1
    fi
  # check for memory leaks on macOS
  elif command -v leaks > /dev/null; then
    tempfile=mktemp
    (MallocStackLogging=1 leaks --quiet --atExit -- ./$output_file) >$tempfile 2>&1
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

  rm $output_file
}

run_tests '../src/zhash.c ./zhash_test.c' 'zhash'
run_tests '../src/zhash.c ../src/zsorted_hash.c ./zsorted_hash_test.c' 'zsorted_hash'
