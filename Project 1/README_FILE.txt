16 & and 8-Bit Emulator

Note: Numbers can be added and subrtacted in two ways in assembly. First method is called regsister addressing mode and the second methood is immediate addressing mode.
This emulator allows the user to:
	1) Move constants and immediates to 8 and 16-bit registers
	2) Add 16-bit register to 16-bit register
	3) Add constant to 16-bit register
	4) Add 8-bit register to 8-bit register
	5) Add constant to 8-bit register
	6) Subtract 16-bit register from a 16-bit register
	7) Subtract constant from 16-bit register
	8) Subtratct 8-bit register from 8-bit register
	9) Subtract constant from 8-bit register
	10) OR, XOR, AND for 16-bit register to 16-bit register 
	11) OR, XOR, AND for constant to 16-bit register
	12) OR, XOR, AND for 8-bit register to 8-bit register
	13) OR, XOR, AND for constant to 8-bit register
	14) ADD, SUBTRACT, OR, XOR, AND for 8-bit constant ro 16-bit register
	15) LOOPS
	16) PUSH and POP numbers to a stack
	17) MULTIPLY and DIVIDE for register to register
	18) INCREMENT AND DECREMENT a number
Note: ZERO FLAG, CARRY FLAG, and SIGN FLAG are also added to the code to ensure that the programs is giving the user the correct output
 
Prerequisites
1. Install DOSBOX on your machine using the following link: https://www.dosbox.com/
2. Install 8086 zip file by clicking the following link: https://drive.google.com/file/d/0B001eXddyriFbFBhclB5QmVvaVE/view

Installing
1. Go, to your Download, click on the 8086 zip file and move the 8086 folder into your C dirve
2. Open your DOSBOX and type in the following commands:
	a) mount c c:/8086
	b) c:
	c) edit filename.asm 
	d) Type in your assembly code here
	e) masm  filename.asm
	f) Hit enter 4 time
	g) link filename.obj
	h) Hit enter 4 times
	i) tasm -l filename.asm
	j) exe2bin filename.exe filename.com (Changing binary to .com file)
	k) td filename.exe

Break down into end to end tests
I created the .COM files (ex, P1E2.COM, PROJ1EX.COM) that with different 
instructions(MOV, ADD, INC, DEC, SUB, PUSH, POP, MUL, DIV) and then printed out the registers to see if my switch statment was working properly.

Give an example
1) Use the steps under, 'Installing'  to create a file called P1E2
2)When you go into P1E2.LST , you see the following:

p1e2.asm



      1				     assume cs:code,ds:code
      2	0000			     code segment
      3					  org 100h
      4	0100			     start:
      5				     ; CODE BEGIN
      6	0100  B8 FFFF			     mov ax,0FFFFh
      7	0103  05 0002			     add ax, 2
      8	0106  50			     push ax
      9	0107  5F			     pop di	    ; di has a value of	1
     10	0108  03 FF			     add di,di	    ; di has a value of	2
     11	010A  8B CF			     mov cx,di	    ; cx = 2
     12	010C  41			     inc cx	    ; cx is 3
     13	010D  2B F9			     sub di, cx	    ; di is FFFF
     14	010F  4F			     dec di	    ; di is FFFE
     15	0110				 L1:
     16	0110  33 FF			     xor di, di	    ; di is 0
     17	0112  33 F8			     xor di, ax	    ; di is 1
     18	0114  57			     push di
     19	0115  83 CF 02			     or	di, 2	    ; di is 2
     20	0118  E2 F6			     LOOP L1
     21	011A  B9 0005			     mov cx, 5
     22
     23	011D				 L2:
     24	011D  2B C7			     sub ax, di	    ; ax is FFFF
     25	011F  50			     push ax
     26	0120  40			     inc ax	    ; ax is 0
     27	0121  5B			     pop bx	    ; bx is FFFF
     28	0122  83 C3 02			     add bx, 2	    ; bx is 1
     29	0125  E0 F6			     LOOPNZ L2	    ; cx is 3 since we did 2 loops
     30	0127  03 CF			     add cx, di	    ; cx is 5
     31	0129				 L3:
     32	0129  83 CB 01			     or	bx,1	    ; bx is 3
     33	012C  23 C3			     and ax, bx	    ; ax is 3
     34	012E  03 F9			     add di, cx	    ; di is 7
     35	0130  2B C3			     sub ax, bx	    ; ax is 0
     36	0132  E1 F5			     LOOPZ L3
     37
     38	0134  CD 20			 int 20h
     39				     ; CODE END
     40	0136			     code ends
     41				     end start

At the end of this code, AX = 10, CX = 1, DX = 0, BX = 0, SP = 65534, BP = 0, SI = 0, DI = 65526, CF = 1, SF = 1, ZF = 0 
To get my output, comment out line 1238 to 1249, and run the proj.cpp file, you should see these values printed in terminal.


Built With
Visual Studio Code - used to write the C++ code
DOSBOX - used to write the assembly code, and get the opcodes needed to write the cases for the switch statement.

Author
Anam Munawar

Acknowledgments
TIP: Get familiar with DOSBOX first, since this will help you come up with cases for the switch statement.