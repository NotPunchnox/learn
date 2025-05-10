; Programme simpler d'instructions

section .data
    nb1 db 10

    err_msg db "Negative !", 10
    err_msg_len equ $ - err_msg

    ok_msg db "Positive !", 10
    ok_msg_len equ $ - ok_msg

section .text
    global _start

_start:
    cmp nb1, 5
    jne .positive
    jmp .negative

.positive:
    mov eax, 4
    mov ebx, 1
    mov ecx, ok_msg
    mov edx, ok_msg_len
    ;init 0x80
    jmp .exit

.negative:
    mov eax, 4
    mov ebx, 1 
    mov ecx, err_msg
    mov edx, err_msg_len
    ;init 0x80
    jmp .exit

.exit:
    mov eax, 1 
    mov ebx, 0 
    int 0x80 
