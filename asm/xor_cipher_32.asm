GLOBAL asm_xor_cipher

section .text

%define syscall int 0x80
%define write 4
%define STDOUT 1

;rc4_cipher_start(void *data, int datalen, char *key, int keylen);



asm_xor_cipher:
    push ebp
    mov ebp, esp

    call debug_print
    call print_newline
    push edi
    push esi
    mov eax , [ebp + 8] ; first param
    mov ebx, [ebp + 12] ; second param
    mov ecx, [ebp + 16] ; third param
    mov edx, [ebp + 20] ; fourth param


	; ecx        ; string to write
    ; edx string len
    mov eax,write
	mov ebx,STDOUT
	syscall              ; call the kernel
    call print_newline
    call print_newline



    jmp _encrypt

_encrypt:
    jmp return

return:
    pop esi
    pop edi
    mov esp, ebp
    pop ebp
    ret


print_newline
    push ebp
    mov ebp, esp

    push eax
    push ebx
    push edx

    sub esp, 4 ; push string to write on stack
    push 10 ;\n

	mov ecx, esp        ; string to write
    mov eax,write
	mov ebx,STDOUT
	mov edx, 4     ; length of string to write
	syscall              ; call the kernel
    add esp, 4

    pop edx
    pop ebx
    pop eax
    mov esp , ebp
    pop ebp
    ret

debug_print:
    call print_newline
    push ebp
    mov ebp, esp

    push eax
    push ebx
    push edx

    sub esp, 8 ; push string to write on stack
    push 'g'
    push 'debu'

	mov ecx, esp        ; string to write
    mov eax,write
	mov ebx,STDOUT
	mov edx, 8     ; length of string to write
	syscall              ; call the kernel
    add esp, 8

    pop edx
    pop ebx
    pop eax
    mov esp , ebp
    pop ebp
    call print_newline
    ret