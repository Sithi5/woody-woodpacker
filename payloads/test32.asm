BITS 32

SECTION .data
        woody_msg: db "...WOODY...",10
        woody_msg_len  : equ $-woody_msg

        debug_msg: db "debug",10
        debug_msg_len  : equ $-woody_msg

SECTION .text

%define syscall int 0x80

_start_payload:
    push eax                 ; save all clobbered registers
    push ecx                 ; (rcx and r11 destroyed by kernel)
    push edx
    push esi
    push edi
    jmp _infection

_infection:
    call _print_woody
    jmp _end_payload

_end_payload:
    pop edi
    pop esi
    pop edx
    pop ecx
    pop eax

    call _ret2oep           ; jump to original entry point(oep)
    push eax
    ret

_debug_write:
    mov eax,1                ; sys_write
    mov edi,1                ; stdout
    mov edx,debug_msg_len;    ; len
    lea esi,[rel $+debug_msg-$]  ; debug msg
    syscall
    ret

_print_woody:
    mov eax,1                ; sys_write
    mov edi,1                ; stdout
    mov edx,woody_msg_len;len
    lea esi,[rel $+woody_msg-$]  ; woody
    syscall
    ret

_get_rip:
    mov eax, dword [esp]
    ret

_ret2oep:
    call _get_rip
    sub eax, 0x77777777 ; virus size without ret2oep
    sub eax, 0x77777777 ; new_entry_point
    add eax, 0x77777777 ; old entry_point
    ret

