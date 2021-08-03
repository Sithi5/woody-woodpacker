#!/bin/sh
rm virus
rm target
nasm -f bin -o hello.bin hello.s
gcc silvio.c -o silvio
gcc -no-pie test1.c -o target
./silvio target hello.bin virus