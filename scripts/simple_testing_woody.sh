#!/bin/sh
make
echo "\nCompiling payload...\n"

sh ./scripts/compile_payload_script.sh

gcc -no-pie ./tests/test1.c -o test1_no_pie

echo "\nCreating and Executing woody with no_pie binary...\n"
./woody_woodpacker test1_no_pie
./woody

rm woody
gcc ./tests/test1.c -o test1
echo "\nCreating and Executing woody with normal binary...\n"
./woody_woodpacker test1
./woody

rm test1
rm test1_no_pie
rm woody
rm woody_woodpacker
rm payload