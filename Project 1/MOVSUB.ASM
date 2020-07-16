assume cs:code,ds:code
code segment
     org 100h
start:
; CODE BEGIN
         mov AX,5
         mov CX,3
         mov DX, 2
         sub AX,1
         sub CX,2
         sub DX,1
     int 20h
; CODE END  
code ends
     end start 