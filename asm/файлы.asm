.model small

.stack 100h

.data
    string      db      80, ?, 80 dup('$')
    length      dw      0
    text        db      80 dup('$')
    char        db      1 dup(?)
    result      db      80 dup('$')
    handle      dw      0;  descriptor 
    value       dw      0
    cin         db      "Enter text:", 13, 10, '$'
    cout        db      "Result: ", 13, 10, '$'
    endl        db      "", 13, 10, '$'
    fname       db      'C:\emu8086\MyBuild\1234.DOCX', 0 

Marc macro t
    local a, b, c, d, end             

a:    
    mov ah, 42h
    mov bx, handle
    mov dx, value
    mov cx, 0
    mov al, 0
    int 21h 
    xor di, di
            
b:
    pop dx
    push dx
    mov ah, 3fh   
    mov bx, handle
    mov cx, 1
    lea dx, char
    int 21h 
      
    inc di  
    xor dx, dx                                                        
    mov al, [t+2+si]
    mov bl, char 
    cmp bl, '$'
    je d             
                  
    cmp bl, al
    je c     
    jne b 

c:
    pop dx
    inc dl 
    push dx
    cmp dl, [t+1]
    je end
    jne d
        
d:
    inc si   
    xor cx, cx
    mov cl, [t+1]
    cmp si, cx
    jl a
    jmp end
    
end:
        
endm                                    

print macro str
    mov ah, 09h
    lea dx, str
    int 21h
endm
    
.code 

begin:
    mov ax, @data
    mov ds, ax                 
    
    mov ah, 3dh
    mov al, 00h
    lea dx, fname
    int 21h   
    mov handle, ax
    
    print cin  
      
    mov ah, 0ah
    lea dx, text
    int 21h          
      
    xor si, si
    xor di, di
    xor dx, dx
    push dx   
             
    Marc text 
    
    mov si, di 
    cmp bl, '$'
    jne o0_0o

e:  
    mov ah, 42h
    mov bx, handle
    mov dx, -1
    mov cx, 0
    mov al, 1
    int 21h 
    

o0_0o:    
    inc si 
    mov ah, 3fh   
    mov bx, handle
    mov cx, 1
    lea dx, char
    int 21h
    mov bl, char
    cmp bl, '$'
    jne  o0_0o

cont:
    inc si
    mov value, si
    pop dx
    mov dh, dl 
    xor dl, dl
    push dx       
    xor si, si 
     
    Marc text
  
    pop dx
    xor ax, ax
    
_dh:
    cmp dh, [text+1]
    je output_dh
    jmp _dl  
     
output_dh:     
    inc al
    
_dl:
    cmp dl, [text+1]
    je output_dl
    jmp continue
             
output_dl:
    inc al     
     
continue:     
    add al, 30h
    mov result, al  
    print endl
    print cout
    print result
    
    
    mov ah, 3eh
    mov bx, handle
    int 21h
    
    mov ah, 4ch
    int 21h 
    
end begin