BITS 64

SECTION .data
        woody_msg: db "...WOODY...",10
        woody_msg_len  : equ $-woody_msg
        key_msg: db "0"
        key_len  : equ $-key_msg

SECTION .text

_start_payload:
    push rax                 ; save all clobbered registers
    push rcx                 ; (rcx and r11 destroyed by kernel)
    push rdx
    push rsi
    push rdi
    push r11
    push r9

_print_woody:
    mov rax,1                ; sys_write
    mov rdi,1                ; stdout
    mov rdx,woody_msg_len;len
    lea rsi,[rel $+woody_msg-$]  ; woody
    syscall


_ret2oep:
    call _get_rip
    sub rax, 0x77777777 ; virus size without ret2oep
    sub rax, 0x77777777 ; new_entry_point
    add rax, 0x77777777 ; old entry_point
    mov r13, rax

_mprotect:
    mov rdi,  r13
    and rdi, -0x1000
    mov rax, 0xa
    mov rsi, 0x1000
    mov rdx, 0x07
    syscall

_getvar:
    mov rdi, r13
    sub rdi, 0x50
    mov r14, 0x1bd
    mov r15, key_len
    lea rsi,[rel $+key_msg-$]

_xorcipher:

mov r9, 00 ; len counter initialisation

_resetkeyloop:
    cmp r9,r14			; check len and len counter
    je 	_end_payload    ; jump if lencounter < len
    lea rsi,[rel $+key_msg-$]
    mov r8, 00          ; key len counter initialisation

_xorloop:
    mov rax, rdi        ; get data char address
    mov bl, byte[rsi]   ; get key byte to xor
    xor byte[rax],bl	; xor operation on data with key


    add rdi,1			; add 1 to pointer data
    add rsi,1			; add 1 to pointer key
    add r9,01			; add 1 to data len counter
    add r8,01           ; add 1 to key len counter
    cmp r8,r15          ; check key to reset
    je _resetkeyloop    ; reset key pointer
    cmp r9,r14			; check len and len counter
    jle _xorloop		; jump if lencounter < len

_end_payload:
    pop r9
    pop r11
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax

_end:
    mov rax, r13
    push rax
    ret

_get_rip:
    mov rax, qword [rsp]
    ret