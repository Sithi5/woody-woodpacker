BITS 64

SECTION .data
        woody_msg: db "...WOODY...",10
        woody_msg_len  : equ $-woody_msg

        debug_msg: db "debug",10
        debug_msg_len  : equ $-woody_msg

SECTION .text
        global _start_payload

_start_payload:
    push rax                 ; save all clobbered registers
    push rcx                 ; (rcx and r11 destroyed by kernel)
    push rdx
    push rsi
    push rdi
    push r11
    jmp _payload

_end_payload
    pop r11
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax

    push 0x401050           ; jump to original entry point
    ret

_payload:
    call .print_woody
    jmp _end_payload

.print_woody:
    mov rax,1                ; sys_write
    mov rdi,1                ; stdout
    mov rdx,woody_msg_len;len
    lea rsi,[rel $+woody_msg-$]  ; woody
    syscall
    ret

.debug:
    mov rax,1                ; sys_write
    mov rdi,1                ; stdout
    mov rdx,debug_msg_len;    ; len
    lea rsi,[rel $+debug_msg-$]  ; debug msg
    syscall
    ret