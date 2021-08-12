#!sh
nasm -f elf64 xor_cipher.asm
gcc maintest.c xor_cipher.o
./a.out
rm *.o
