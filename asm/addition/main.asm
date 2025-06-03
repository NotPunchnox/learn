; Section variables
.data
resultat_msg db "Le résultat est : ", 0
resultat_len equ $ - resultat_msg
msg db 10, 0
len equ $ - msg
resultat db 0

; Effectuer l'addition ( 10 + 25 )
mov eax, 10   ; Premier nombre
mov ebx, 25   ; Deuxième nombre
add eax, ebx  ; Additionner les deux nombres

; Convertir le résultat en char ASCII
add eax, '0'
mov [resultat], al

; Afficher le résultat
mov eax, 4              ; Appel système pour écrire
mov ebx, 1              ; Sortie standard
mov edx, resultat_msg   ; Pointer vers le message
mov ecx, resultat_len   ; Longueur du message
int 0x80                ; Appel système

; Ajouter un saut de ligne
mov eax, 4
mov ebx, 1
mov ecx, msg
mov edx, 1      ; longueur du saut de ligne
add ecx, len-1  ; pointe vers la char de saut de ligne
int 0x80

; Quitter le programme
mov eax, 1              ; Appel système pour quitter
mov ebx, 0              ; Code de sortie
int 0x80                ; Appel système
