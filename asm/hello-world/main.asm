; Programme simple pour afficher un message en assembleur x86 (NASM, Linux)

section .data
    message db "Bonjour, monde!", 10  ; Message avec saut de ligne
    message_len equ $ - message       ; Longueur du message


section .text
    global _start

_start:
    ; Afficher le message
    mov eax, 4          ; sys_write
    mov ebx, 1          ; stdout
    mov ecx, message    ; Pointeur vers le message
    mov edx, message_len ; Longueur du message
    int 0x80            ; Appel système

    ; Quitter le programme
    mov eax, 1          ; sys_exi:t
    mov ebx, 0          ; Code de retour 0
    int 0x80            ; Appel système
