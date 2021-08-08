BITS 64

SECTION .data
        woody_msg: db "...WOODY...",10
        woody_msg_len  : equ $-woody_msg

SECTION .text
        global _start

_start:
    push rax                 ; save all clobbered registers
    push rcx                 ; (rcx and r11 destroyed by kernel)
    push rdx
    push rsi
    push rdi
    push r11

    xor rdx,rdx
    mov rax,1                ; sys_write
    mov rdi,1                ; stdout
    mov rdx,woody_msg_len;[rel $+len-$]    ; len
    lea rsi,[rel $+woody_msg-$]  ; hello
    syscall

    pop r11
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax

    jmp .ret2oep           ; jump to original entry point
    ret


.get_rip:
    mov rax, qword [rsp]
    ret

.ret2oep:
    call .get_rip
    sub rax, 0x77
    sub rax, 0x77777777
    add rax, 0x77777777
    jmp rax