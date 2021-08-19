BITS 32

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

_get_rip:
    mov eax, dword [esp]
    ret

_ret2oep:
    call _get_rip
    sub eax, 0x77777777 ; virus size without ret2oep
    sub eax, 0x77777777 ; new_entry_point
    add eax, 0x77777777 ; old entry_point
    ret

_print_woody:
    ; Setting the string woody on stack
    sub esp, 0xf
    push 10
    push '...'
    push 'OODY'
    push '...W'
	mov ecx, esp        ; string to writef

    mov eax,4            ; 'write' system call = 4
	mov ebx,1            ; file descriptor 1 = STDOUT
	mov edx, 13     ; length of string to write
	syscall              ; call the kernel

    add esp, 0xf
    pop edi
    pop edi
    pop edi
    pop edi
    ret
