 section .data
        WoodyMsg:     db '....WOODY....\n',14
        WoodyMsgSize:    equ $-WoodyMsg
 section .text
        global _start
 _start:
        mov eax,4             ; Appel système "write" (sys_write)
        mov ebx,1             ; File descriptor, 1 pour STDOUT (sortie standard)
        mov ecx, WoodyMsg     ; Adresse de la chaîne a afficher
        mov edx, WoodyMsgSize ; Taille de la chaîne
        int 80h               ; Exécution de l'appel système
                              ; Sortie du programme
        mov eax,1             ; Appel système "exit"
        mov ebx,0             ; Code de retour
        int 80h