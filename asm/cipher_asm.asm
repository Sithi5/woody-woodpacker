rc4_cipher_asm:

    push rbp
    mov rbp, rsp

    mov r14, rcx ; store data len
    mov r15, rdx ; store key len

    push rsi ; store arg pointeur to data
    push rdi ; store arg pointeur to key
    mov r9, 00 ; set index i to 0
    mov r10, 00 ; set index j to 0
    sub	rsp, 0x100

init_stream:
    cmp r9, 255
    je keystream_generation
    ;mov rsp + r9, r9
    add r9, 1
    jmp init_stream

keystream_generation:
    mov r9, 00
    jmp return

loop_1:
    ret




return:
    pop rsi             ; remove key address from stack
    pop rdi				; retreave data from the stack
    mov rax, rdi		; set return address to rdi (data)
    mov rsp,rbp
    pop rbp
    ret



section .data

section .text
    GLOBAL rc4_cipher