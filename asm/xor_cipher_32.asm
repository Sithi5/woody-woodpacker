GLOBAL asm_xor_cipher

section .text

%define syscall int 0x80
%define write 4
%define STDOUT 1

;rc4_cipher_start(void *data, int datalen, char *key, int keylen);



asm_xor_cipher:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 20] ; keylen
    dec edx ; keylen
    mov ecx, [ebp + 12] ; datalen
    dec ecx ; datalen
    jmp _encrypt

_encrypt:

    mov eax, [ebp + 8] ; data
    mov ebx, [ebp + 16] ; key

    cmp edx, 0
    jge continue
    mov edx, [ebp + 20] ; keylen
    dec edx ; dec keylen
    continue:
    mov esi, [eax + ecx]
    mov bl, byte[ebx + edx] ; getting key char
    xor byte[eax + ecx], bl

    dec edx ; dec keylen
    dec ecx ; datalen
    cmp ecx, 0
    jge _encrypt

return:
    mov esp, ebp
    pop ebp
    ret


print_newline:
    push ebp
    mov ebp, esp

    push eax
    push ebx
    push ecx
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
    pop ecx
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
    push ecx
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
    pop ecx
    pop ebx
    pop eax
    mov esp , ebp
    pop ebp
    call print_newline
    ret