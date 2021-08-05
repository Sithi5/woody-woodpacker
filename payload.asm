BITS 64

SECTION .text
global start

hello: db "...WOODY...",10,0
len  : dd 13

start:
  push rax                 ; save all clobbered registers
  push rcx                 ; (rcx and r11 destroyed by kernel)
  push rdx
  push rsi
  push rdi
  push r11

  xor rdx,rdx
  mov rax,1                ; sys_write
  mov rdi,1                ; stdout
  mov rdx,13;[rel $+len-$]    ; len
  lea rsi,[rel $+hello-$]  ; hello
  syscall


