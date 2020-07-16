assume cs:code,ds:code
code segment
     org 100h
start:
; CODE BEGIN
        mov ax,0FFFFh
        add ax, 2      
        push ax
        pop di         ; di has a value of 1
        add di,di      ; di has a value of 2
        mov cx,di      ; cx = 2
        inc cx         ; cx is 3
        sub di, cx     ; di is FFFF
        dec di         ; di is FFFE
    L1:
        xor di, di     
        xor di, ax     
        push di
        or di, 2       
        LOOP L1
        mov cx, 5

    L2:
        sub ax, di     
        push ax
        inc ax         
        pop bx         
        add bx, 2      
        LOOPNZ L2      
        add cx, di     

    L3:
        or bx,1        
        and ax, bx     
        add di, cx     
        sub ax, bx     
        LOOPZ L3
        
    int 20h
; CODE END  
code ends
end start 