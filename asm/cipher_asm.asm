
section .data

section .bss
    global stream_cpy

    stream resb 256
    stream_cpy resb 256

section .text
    GLOBAL rc4_cipher_start


rc4_cipher_start:
    ;rdi = start offset
    ;rsi = size
    ;rdx = key
    ;rcx = key_size

    lea r8, [rel stream]			; stream 
    cmp byte [r8 + 1], 0			; check if stream has to be init
    jne encrypt_reset_vals			; if yes, skip initialisation
    xor r9, r9
    mov r13, rdx					; r13 = key

init_stream_default_values:
    cmp r9, 256						; for (i = 0; i < 256; i++)
    je init_stream_reset_vals
    mov [r8 + r9], r9b				; stream[i] = i;
    inc r9
    jmp init_stream_default_values

init_stream_reset_vals:
    xor r9, r9						; i
    xor r10, r10					; j
    xor r11, r11					; tmp_i
    xor r12, r12					; tmp_j

init_stream_real_values:
    cmp r9, 256						; for (i = 0; i < 256; i++)
    je copy_stream
    add r10b, [r8 + r9]				; j = (j + stream[i]

    xor rdx, rdx					; rdx is remainder
    mov rax, r9						; rax = i
    div rcx							; rcx = key_size : rax / rcx -> rdx = %

    add r10b, [r13 + rdx]			; + key[i % key_size])
    mov r11b, [r8 + r9]				; tmp_i = stream[i]
    mov r12b, [r8 + r10]			; tmp_j = stream[j]
    mov [r8 + r10], r11b			; stream[j] = stream[i]
    mov [r8 + r9], r12b				; stream[i] = tmp_j

    inc r9
    jmp init_stream_real_values

copy_stream:
    ;save rdi, rsi and rcx
    push rdi
    push rsi
    push rcx

    lea rdi, [rel stream_cpy]		; dest
    lea rsi, [rel stream]			; src
    mov rcx, 256					; n
    rep movsb						; memcpy

    ;restore rdi, rsi ans rcx
    pop rcx
    pop rsi
    pop rdi

encrypt_reset_vals:
    xor r9, r9						; i
    xor r10, r10					; j
    xor r11, r11					; tmp_i
    xor r12, r12					; tmp_j
    xor r13, r13					; k
    xor r14, r14					; K
    xor r15, r15					; l

encrypt:
    cmp r15, rsi					; while (l < size)
    jge end
    add r9b, 1						; i = (i + 1) % 256
    add r10b, [r8 + r9]				; j = j + stream[i]
    mov r11b, [r8 + r9]				; tmp_i = stream[i]
    mov r12b, [r8 + r10]			; tmp_j = stream[j]
    mov [r8 + r10], r11b			; stream[j] = tmp_i
    mov [r8 + r9], r12b				; stream[i] = tmp_j

    mov r13b, [r8 + r9]				; k = stream[i]
    add r13b, [r8 + r10]			; k = k + stream[j]
    mov r14b, [r8 + r13]			; K = stream[k]
    xor [rdi + r15], r14b			; byte to encrpt ^ K
    inc r15							; l++
    jmp encrypt

end: