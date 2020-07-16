# 16 & and 8-Bit Emulator

Note: Numbers can be added and subrtacted in two ways in assembly. First method is called regsister addressing mode and the second methood is immediate addressing mode.
This emulator allows the user to: </br>
	1) Move constants and immediates to 8 and 16-bit registers </br>
	2) Add 16-bit register to 16-bit register </br>
	3) Add constant to 16-bit register </br>
	4) Add 8-bit register to 8-bit register </br>
	5) Add constant to 8-bit register </br>
	6) Subtract 16-bit register from a 16-bit register </br>
	7) Subtract constant from 16-bit register </br>
	8) Subtratct 8-bit register from 8-bit register </br>
	9) Subtract constant from 8-bit register </br>
	10) OR, XOR, AND for 16-bit register to 16-bit register </br>
	11) OR, XOR, AND for constant to 16-bit register </br>
	12) OR, XOR, AND for 8-bit register to 8-bit register </br>
	13) OR, XOR, AND for constant to 8-bit register </br>
	14) ADD, SUBTRACT, OR, XOR, AND for 8-bit constant ro 16-bit register </br>
	15) LOOPS </br>
	16) PUSH and POP numbers to a stack </br>
	17) MULTIPLY and DIVIDE for register to register </br>
	18) INCREMENT AND DECREMENT a number </br>
Note: ZERO FLAG, CARRY FLAG, and SIGN FLAG are also added to the code to ensure that the programs is giving the user the correct output </br> </br>
 
### Prerequisites </br>
1. Install DOSBOX on your machine using the following link: https://www.dosbox.com/ </br>
2. Install 8086 zip file by clicking the following link: https://drive.google.com/file/d/0B001eXddyriFbFBhclB5QmVvaVE/view </br>

### Installing
1. Go, to your Download, click on the 8086 zip file and move the 8086 folder into your C dirve </br>
2. Open your DOSBOX and type in the following commands: </br>
	a) mount c c:/8086  </br>
	b) c: </br>
	c) edit filename.asm </br>
	d) Type in your assembly code here </br>
	e) masm  filename.asm </br>
	f) Hit enter 4 time </br>
	g) link filename.obj </br>
	h) Hit enter 4 times </br>
	i) tasm -l filename.asm </br>
	j) exe2bin filename.exe filename.com (Changing binary to .com file) </br>
	k) td filename.exe </br> </br>

### Break down into end to end tests </br>
I created the .COM files (ex, P1E2.COM, PROJ1EX.COM) that with different 
instructions(MOV, ADD, INC, DEC, SUB, PUSH, POP, MUL, DIV) and then printed out the registers to see if my switch statment was working properly. </br> </br>

### Give an example
1) Use the steps under, 'Installing'  to create a file called P1E2 </br>
2) When you go into P1E2.LST , you see the following: </br>
