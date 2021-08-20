GLOBAL asm_xor_cipher

section .text

%define syscall int 0x80
%define write 4
%define STDOUT 1

;rc4_cipher_start(void *data, int datalen, char *key, int keylen);



asm_xor_cipher:
    push ebp ; Sauvegarde de l’ancien bas de pile
    mov ebp, esp ; on remonte le bas de pile a la position du haut de la pile

    mov edx, [ebp + 20] ; keylen
    dec edx ; keylen
    mov ecx, [ebp + 12] ; datalen
    dec ecx ; datalen
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
    push ebp ; Sauvegarde de l’ancien bas de pile
    mov ebp, esp ; on remonte le bas de pile a la position du haut de la pile

    push eax
    push ebx
    push ecx
    push edx

    call print_newline

    ; Pushing string on stack
    sub esp, 8 ; get 8 bytes on stack to push string on it
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
    add esp, 8


    call print_newline

    pop edx
    pop ecx
    pop ebx
    pop eax

    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret

print_newline:
    push ebp ; Sauvegarde de l’ancien bas de pile
    mov ebp, esp ; on remonte le bas de pile a la position du haut de la pile

    push eax
    push ebx
    push ecx
    push edx

    ; Pushing string on stack
    sub esp, 4 ; get 4 bytes on stack to push string on it
    push 10

	mov ecx, esp        ; string to write
    mov eax,write
	mov ebx,STDOUT
	mov edx, 4     ; length of string to write
	syscall              ; call the kernel

    ; Removing string on stack to restore it.
    pop eax
    pop eax
    add esp, 4

    pop edx
    pop ecx
    pop ebx
    pop eax

    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret
