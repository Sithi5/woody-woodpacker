BITS 32

SECTION .text
%define syscall int 0x80
%define write 4
%define STDOUT 1

_start_payload:
    enter 0,0 ; push ebp, mov ebp, sub esp, N
    push eax                 ; save all clobbered registers
    push ecx                 ; (rcx and r11 destroyed by kernel)
    push edx
    push esi
    push edi

_infection:
    call _print_woody

_end_payload:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop eax

    leave
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

; _ret2textsection:
;     call _get_rip
;     sub eax, 0x66666666 ; virus size without ret2oep
;     sub eax, 0x66666666 ; new_entry_point
;     add eax, 0x66666666 ; start of text section
;     ret

; _ret2textoffset:
;     mov eax, 0x55555555
;     ret

; _mprotect:
;     call _ret2textsection
;     mov edi,  eax
;     and edi, -0x1000
;     call _ret2textoffset
;     mov esi, eax
;     mov eax, 0xa
;     mov edx, 0x07
;     syscall
;     ret

_print_woody:
    enter 0,0 ; push ebp, mov ebp, esp

    ;Save registers on stack
    push eax
    push ebx
    push ecx
    push edx

    ; Pushing string on stack
    push 10
    push '...'
    push 'OODY'
    push '...W'

    ; do write call
	mov ecx, esp        ; string to write
    mov eax,write
	mov ebx,STDOUT
	mov edx, 16     ; length of string to write
	syscall              ; call the kernel

    ; Removing string on stack to restore it.
    pop eax
    pop eax
    pop eax
    pop eax

    ;Get back registers from stack
    pop edx
    pop ecx
    pop ebx
    pop eax

    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret