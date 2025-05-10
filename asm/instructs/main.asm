; Programme simpler d'instructions

section .data
    nb1 db 10, 10
    len equ $ - nb1
    
    err_msg db "Negative !", 10
    err_msg_len equ $ - err_msg

    ok_msg db "Positive !", 10
    ok_msg_len equ $ - ok_msd

section .text
    global _start

_start:
    ;cmp nb1, 5
    ;jne .positive
    ;jpn .negative

    

.afficher:
    mov eax, 4
    mov ebx, 1 
    mov ecx, message
    mov edx, message_len
    init 0x80

.exit:
    mov eax, 1 
    mov ebx, 0 
    int 0x80 
