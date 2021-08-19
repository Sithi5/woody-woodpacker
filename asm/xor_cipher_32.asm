section .text
    GLOBAL asm_xor_cipher

asm_xor_cipher:

    push ebp
    mov ebp, esp

    ;ecx ; keylen 4 param
    ;edx ;len 3 param
    ;esi ;key 2 param
    ;edi ;data 1 param

    mov r14, edx ; store data len
    mov r15, ecx ; store key len

    extern puts
    push edi ; store arg pointeur to data
    push esi ; store arg pointeur to key

    mov r9, 00 ; len counter initialisation

resetkeyloop:
    cmp r9,r14d			; check len and len counter
    je 	return		    ; jump if lencounter < len
    pop esi             ; get start of key
    push esi            ; send back start of key in stack
    mov r8, 00          ; key len counter initialisation

xorloop:
    mov eax, edi        ; get data char address
    mov bl, byte[esi]   ; get key byte to xor
    xor byte[eax],bl	; xor operation on data with key


    add edi,1			; add 1 to pointer data
    add esi,1			; add 1 to pointer key
    add r9,01			; add 1 to data len counter
    add r8,01           ; add 1 to key len counter
    cmp r8,r15          ; check key to reset
    je resetkeyloop     ; reset key pointer
    cmp r9,r14			; check len and len counter
    jle xorloop			; jump if lencounter < len



; return function
return:
    pop esi             ; remove key address from stack
    pop edi				; retreave data from the stack
    mov eax, edi		; set return address to edi (data)
    mov esp,ebp
    pop ebp
    ret