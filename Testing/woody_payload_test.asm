section .data
    WoodyMsg:     db '....WOODY....',10,0
    WoodyMsgSize:    equ $-WoodyMsg
    format: db '%s',0
section .text
    extern printf

    global main

main:
    push rbp
    mov rbp,rsp
    mov rdi, format
    mov rsi, WoodyMsg
    call printf
    leave
    ret                     ; Sortie du programme