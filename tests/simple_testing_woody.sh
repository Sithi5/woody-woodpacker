#!/bin/sh
make
gcc -no-pie ./tests/test1.c -o test1_no_pie
gcc ./tests/test1.c -o test1
echo "\nCreating and Executing woody with no_pie binary...\n"
./woody_woodpacker tests/test1_no_pie
./woody
rm woody
echo "\nCreating and Executing woody with normal binary...\n"
./woody_woodpacker tests/test1
./woody
rm test1
rm test1_no_pie
rm woody
rm woody_woodpacker