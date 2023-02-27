.model small 
.stack 100h
.data
.code

Start:
        ;; initialization of com1  ;; 
       
        mov dx, 3fBh
        mov al, 0dbh
        out dx, al

        mov dx, 3f8h
        mov al, 01h
        out dx, al  
        
        mov al, 00h
        mov dx, 3F9h
        out dx, al

        ;; Send byte to COM1 ;;
        mov dx, 3F8h
        mov al, 'A'
        out dx, al

        ;; Read Byte from COM2 ;;
        mov dx, 2F8h
        in al, dx
        mov dl, al
        mov ah, 02h
        int 21h

        mov ax, 4c00h
        int 21h
end Start