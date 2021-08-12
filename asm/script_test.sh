#!sh
nasm -f elf64 cipher_asm.asm
gcc maintest.c cipher_asm.o
./a.out
rm *.o
