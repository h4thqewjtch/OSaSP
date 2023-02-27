.model small 
.stack 100h

.data
.code

start:
        xor ax, ax
        mov ax, 00ebh
        mov dx, 0h
        int 14h

        mov al, 'A'
        mov ah, 1
        mov dx, 0
        int 14h

        xor ax, ax

        mov ah, 2
        mov dx, 1
        int 14h

        mov ah, 2h
        mov dl, al
        int 21h

        mov ax, 4C00h
        int 21h

end start