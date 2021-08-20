GLOBAL asm_xor_cipher

section .text

%define syscall int 0x80
%define write 4
%define STDOUT 1

;rc4_cipher_start(void *data, int datalen, char *key, int keylen);



asm_xor_cipher:
    enter 0,0 ; push ebp, mov ebp, sub esp, N

    mov edx, [ebp + 20] ; keylen
    dec edx ; keylen
    mov ecx, [ebp + 12] ; datalen
    dec ecx ; datalen
    call debug_print
    jmp _encrypt

_encrypt:

    mov eax, [ebp + 8] ; data
    mov ebx, [ebp + 16] ; key

    cmp edx, 0
    jge .continue
    mov edx, [ebp + 20] ; keylen
    dec edx ; dec keylen
    .continue:

    mov esi, [eax + ecx]
    mov bl, byte[ebx + edx] ; getting key char
    xor byte[eax + ecx], bl

    dec edx ; dec keylen
    dec ecx ; datalen
    cmp ecx, 0
    jge _encrypt

return:
    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret


debug_print:
    enter 0,0 ; push ebp, mov ebp, sub esp, N

    push eax
    push ebx
    push ecx
    push edx

    call print_newline

    ; Pushing string on stack
    push 'g'
    push 'debu'

	mov ecx, esp        ; string to write
    mov eax,write
	mov ebx,STDOUT
	mov edx, 8     ; length of string to write
	syscall              ; call the kernel

    ; Removing string on stack to restore it.
    pop eax
    pop eax


    call print_newline

    pop edx
    pop ecx
    pop ebx
    pop eax

    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret

print_newline:
    enter 0,0 ; push ebp, mov ebp, sub esp, N

    push eax
    push ebx
    push ecx
    push edx

    ; Pushing string on stack
    push 10

	mov ecx, esp        ; string to write
    mov eax,write
	mov ebx,STDOUT
	mov edx, 4     ; length of string to write
	syscall              ; call the kernel

    ; Removing string on stack to restore it.
    pop eax

    pop edx
    pop ecx
    pop ebx
    pop eax

    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret
