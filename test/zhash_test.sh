#!/bin/bash

gcc -Wall -Wextra -g ../src/zhash.c ./zhash_test.c -o hash

./hash && valgrind --quiet --leak-check=yes ./hash

rm hash
