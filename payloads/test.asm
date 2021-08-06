BITS 64

SECTION .data
        woody_msg: db "...WOODY...",10
        woody_msg_len  : equ $-woody_msg

        debug_msg: db "debug",10
        debug_msg_len  : equ $-woody_msg

SECTION .bss
        old_entry_point_var: resb 16

SECTION .text
        global _start_payload



_start_payload:


    push rax                 ; save all clobbered registers
    push rcx                 ; (rcx and r11 destroyed by kernel)
    push rdx
    push rsi
    push rdi
    push r11

    call .print_woody

    call .debug

    ; mov rax, 0x401050
    ; mov [old_entry_point_var], rax

    call .debug

    pop r11
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax

    push 0x401050           ; jump to original entry point
    ret

.print_woody:
    mov rax,1                ; sys_write
    mov rbx,1                ; stdout
    mov rcx,woody_msg  ; woody_msg
    mov rdx,woody_msg_len; len
    syscall
    ret

.debug:
    mov rax,1                ; sys_write
    mov rbx,1                ; stdout
    mov rcx,debug_msg  ; woody_msg
    mov rdx,debug_msg_len; len
    syscall
    ret