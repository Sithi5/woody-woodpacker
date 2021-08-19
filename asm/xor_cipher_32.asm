GLOBAL asm_xor_cipher

section .text

%define syscall int 0x80
%define write 4
%define STDOUT 1

;rc4_cipher_start(void *data, int datalen, char *key, int keylen);



asm_xor_cipher:
    push ebp
    mov ebp, esp


    mov ecx, [ebp + 12] ; datalen
    jmp _encrypt

minus:
    sub eax, ebx
modulo:
    CMP eax, ebx
    JGE minus
    leave   ; else result is already saved in eax
    ret

_encrypt:

    ;getting char from data
    mov eax, [ebp + 12] ; get datalen arg
    sub eax, ecx ; sub to current index
    mov ebx, [ebp + 8] ; data
    add ebx, eax ; data[index]
    mov al, [ebx]   ; save data[index] in al

    ;getting char from key
    mov eax, [ebp + 12] ; get datalen arg
    sub eax, ecx ; sub to current index
    mov ebx, [ebp + 20] ; keylen
    call modulo ; eax % ebx, actually modulo index to keylen
    mov ebx, [ebp + 16] ; key
    add ebx, eax

    xor byte[al], ebx


    loop _encrypt
    jmp return

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