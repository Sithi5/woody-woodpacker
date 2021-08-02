BITS 64

SECTION .text
global start
 
start:
  push rax                 ; save all clobbered registers
  push rcx                 ; (rcx and r11 destroyed by kernel)
  push rdx
  push rsi
  push rdi
  push r11

  mov rax,1                ; sys_write
  mov rdi,1                ; stdout
  lea rsi,[rel $+hello-$]  ; hello
  mov rdx,[rel $+len-$]    ; len
  syscall

  pop r11
  pop rdi
  pop rsi
  pop rdx
  pop rcx
  pop rax

  push 0x401040           ; jump to original entry point
  ret

hello: db "hello world",33,10
len  : dd 13

