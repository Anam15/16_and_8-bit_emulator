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
        sub di, cx     ; di is FF
        dec di         ; di is FE
    L1:
        xor di, di     ; di is 0
        xor di, ax     ; di is 1
        push di
        or di, 2       ; di is 2
        LOOP L1
     int 20h
; CODE END  
code ends
end start 