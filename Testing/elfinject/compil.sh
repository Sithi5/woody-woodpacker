#!/bin/sh
nasm -f bin -o hello.bin hello.s
gcc elfinject.c -o elfinject -lelf
gcc -no-pie test1.c -o testz