#!/bin/bash

gcc -Wall -g ../src/zhash.c ./zhash_test.c -o hash

./hash && valgrind --quiet --leak-check=yes ./hash

rm hash
