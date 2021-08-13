
section .data

section .bss
    global stream_cpy

    stream resb 256
    ;stream_cpy resb 256

section .text
    GLOBAL rc4_cipher_start

_swap:
    mov r11b, [r8 + r9]          ; tmp_i = stream[i]
    mov r12b, [r8 + r10]         ; tmp_j = stream[j]
    mov [r8 + r10], r11b         ; stream[j] = stream[i]
    mov [r8 + r9], r12b          ; stream[i] = tmp_j
    mov rax, r8
    ret

rc4_cipher_start:
    ;rdi = start offset
    ;rsi = size
    ;rdx = key
    ;rcx = key_size
    mov r14, rdx                    ; r14 = key
    lea r8, [rel stream]            ; load stream to memory
    xor r9, r9                      ; int i = 0

init_stream:
    cmp r9, 256                     ; while i < 256
    jne init_index_values             ; reset i and j when i = 256 and continue
    mov [r8 + r9], r9b              ; key_tab[i] = i;
    add r9, 1                       ; i++
    jmp init_stream

init_index_value:
    xor r9, r9                      ; int i
    xor r10, r10                    ; int j
    xor r11, r11                    ; int tmp_i for swap
    xor r12, r12                    ; int tmp_j for swap

stream_generation:
    cmp r9, 256                     ; while i < 256
    jne reset_index_values
    add r10b, [r8 + r9]             ; j = j + stream[i] (using r10b for modulo 256)
    xor rdx, rdx                    ; remainder of %
    mov rax, r9                     ; div using rax for diviser
    div rcx                         ; rax / rcx = rdx = i % rdx
    add r10b, [r14 + rdx]           ; + key[i % key_size]
    call _swap
    add r9 + 1                      ; i + 1

reset_index_values:
    xor r9, r9                      ; int i
    xor r10, r10                    ; int j
    xor r11, r11                    ; int tmp_i for swap
    xor r12, r12                    ; int tmp_j for swap
    xor r13, r13                    ; int k
    xor r15, r15                    ; int res

encrypt:
    cmp r13, rsi                    ; while k < len
    jne return
    add r9b, [r9 + 1]               ; i = (i + 1) % N;
    add r10b, [r8 + r9]             ; j = (j + stream[i]) % N
    call _swap
    mov r14b, [r8 + r9b]
    add r14b, [r8 + r10b]
    mov r15, [r8 + r14b]
    xor [rdi + r13], r15
    add r13, 1                      ; k++
    jmp encrypt

return:
    pop rsi
    pop rdx
    pop rcx
    mov rax, rdi
    pop rdi
    ret




