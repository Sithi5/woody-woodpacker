#!/bin/sh

BASE_FILE_NAME=woody_payload_test

nasm -f elf64 $BASE_FILE_NAME.asm

objdump -d $BASE_FILE_NAME.o
echo ''
objdump -d $BASE_FILE_NAME.o | cut -d ' ' -f2 | tr '\n' ' ' | sed $'s/^.*_main>:/shellcode::::::\\\n/' | tr -s ' ' | sed 's/ /\\x/g' | sed 's/......$//'
echo '\n'

LENGTH=`objdump -d $BASE_FILE_NAME.o | tail -1 | cut -f1 | sed 's/.$//' | sed 's/ *//'`
LENGTHDEC=$(echo "obase=10; ibase=16; $LENGTH" | bc)
printf "lenght = 0x%d // %d \n" $LENGTH $LENGTHDEC
rm woody_payload_test.o