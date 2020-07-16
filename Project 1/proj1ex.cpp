#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <stdio.h>

using namespace std;

// 16-bit rehisters defined
#define AX regis[0]
#define CX regis[1]
#define DX regis[2]
#define BX regis[3]
#define SP regis[4]
#define BP regis[5]
#define SI regis[6]
#define DI regis[7]

//8-bit registers defined
#define AL regis8[0]
#define CL regis8[1]
#define DL regis8[2]
#define BL regis8[3]
#define AH regis8[4]
#define CH regis8[5]
#define DH regis8[6]
#define BH regis8[7]

//function for updateing my 8-bit registers ---> everytime I update my al and ah, my ax needed to be updated
void updateRegis8(unsigned short ax, unsigned short cx, unsigned short dx, unsigned short bx, unsigned short& al, unsigned short& ah, 
        unsigned short& cl, unsigned short& ch, unsigned short& dl, unsigned short& dh, unsigned short& bl, unsigned short& bh)
        {
            ah = (ax/256)*256;
            ch = (cx/256)*256;   
            dh = (dx/256)*256;                            
            bh = (bx/256)*256;
            al = ax - ah;
            cl = cx - ch;   
            dl = dx - dh;             
            bl = bx - bh;
        }

//function for updating my 16-bit registers ---> everytime I update my ax, my al and ah needs to be updated
void updateRegis16(unsigned short& ax, unsigned short& cx, unsigned short& dx, unsigned short& bx, unsigned short al, unsigned short ah, 
        unsigned short cl, unsigned short ch, unsigned short dl, unsigned short dh, unsigned short bl, unsigned short bh)
        {
            ax = ah+al;
            cx = ch+cl;
            dx = dh+dl;
            bx = bh+bl;                   
                    
        }

//Zero Flag 
unsigned short ZF = 0;
void zero_flag(unsigned short Z, unsigned short& ZF){
    if(Z==0) ZF = 1;
    else ZF = 0;
}
//sign flag is on, when the most significant bit is 1, otherwise it's 0
unsigned short SF;
void sign_flag(unsigned short S, unsigned short& SF){
    //higher registers
    if(S >=32768) SF=1;
    //lower registers
    else if(S >=128) SF = 1;
    else SF = 0;
}

//carry flag ----> for unsigned bit, overflow----> for signed bit, auxiliary flag ---> binary coded decimal
unsigned short CF;
unsigned short T; //temporary variable
int typeOp;
void carry_flag(unsigned short C, unsigned short& CF, unsigned short t, int typeOp){
    if(typeOp == 1){ //add
        if (t > C){
            CF = 1;
        }
        else 
            CF = 0; 
    }
    else
    {//subtract
        if(t<C)
            CF =1;
        else
            CF = 0;              
    }
    
}

int main()
{
    int initial_pointer = 0x100; //the program written in RAM, starting with address 100
    int stack[100000]; // initializing stack for push and pop
    unsigned char memory[100000]; //initializing memory
    unsigned short regis[8] = {0,0,0,0,0xFFFE,0,0,0}; //initialize 16-bit register
    unsigned short regis8[8] = {0,0,0,0,0xFFFE,0,0,0}; //initialize 8-bit register
    unsigned short IP = 0x100; 
       
    FILE *filee = fopen("SAMPLE.COM", "rb"); //opens the file, change "SAMPLE.COM" to any file you want to run
    
    if (!filee) { //cheking if the file exists or not
        cout<<("ERROE: No file found")<<endl;
    }
    else
    {
        fseek(filee,0,SEEK_END);
        long ss = ftell(filee);
        fseek(filee,0,SEEK_SET);
        fread(initial_pointer + memory, 1, ss, filee);
        fclose(filee);

        unsigned char byte;

        while(1)
        {
            switch (byte = memory[IP++])
            {
                updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                //increment (16-bit register)
                case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
                    regis[byte&7]++;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(regis[byte&7],ZF); //setting zero flag
                break;

                //decrement(16-bit register)
                case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
                    regis[byte&7]--;
                    zero_flag(regis[byte&7],ZF); //setting zero flag
                break;

         
                //Moving constants/immediates to 8-bit register
                case 0xB0:
                    AL = memory[IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
                case 0xB1:
                    CL = memory[IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
                case 0xB2:
                    DL = memory[IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
                case 0xB3:
                    BL = memory[IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
                 case 0xB4:
                    AH = memory[IP++] *256;
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
                 case 0xB5:
                    CH = memory[IP++] *256;
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
                 case 0xB6:
                    CH = memory[IP++] *256;
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;                
                 case 0xB7:
                    CH = memory[IP++] *256;
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;                

                //MOV for 16bit registers --> AX,CX, DX,......
                case 0xB8:
                    AX = memory[IP++] + (memory[IP++]*256);
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    IP += 2;
                    break;
                 case 0xB9:
                    CX = memory[IP++] + (memory[IP++]*256);
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    IP += 2;
                    break;  
                case 0xBA:
                    DX = memory[IP++] + (memory[IP++]*256);
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    IP += 2;
                    break;   
                case 0xBB:
                    AX = memory[IP++] + (memory[IP++]*256);
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    IP += 2;
                    break;   
                case 0xBC:
                    AX = memory[IP++] + (memory[IP++]*256);
                    IP += 2;
                    break;                               
                case 0xBD:
                    AX = memory[IP++] + (memory[IP++]*256);
                    IP += 2;
                    break;      
                case 0xBE:
                    AX = memory[IP++] + (memory[IP++]*256);
                    IP += 2;
                    break; 
                case 0xBF:
                    AX = memory[IP++] + (memory[IP++]*256);
                    IP += 2;
                    break;   

                case 0x8A:
                        for (unsigned char i = 0; i < 4; i++){  //note: AH*256
                        //moving lower 8bits
                            //AL-BL
                            if (memory[IP]==0xC0+i) AL = regis8[i];
                            else if (memory[IP]==0xC8+i) CL = regis8[i];
                            else if (memory[IP]==0xD0+i) DL = regis8[i];
                            else if (memory[IP]==0xD8+i) BL = regis8[i];
                            //AH-BH
                            else if (memory[IP]==0xE0+i) AH = regis8[i]*256;
                            else if (memory[IP]==0xE8+i) CH = regis8[i]*256;
                            else if (memory[IP]==0xF0+i) DH = regis8[i]*256;
                            else if (memory[IP]==0xF8+i) BH = regis8[i]*256;
                            //moving higher 8bits   like: MOV AL,AH
                            //AL-BL
                            else if (memory[IP]==0xC4+i) AL = regis8[i+4]/256;
                            else if (memory[IP]==0xCC+i) CL = regis8[i+4]/256;
                            else if (memory[IP]==0xD4+i) DL = regis8[i+4]/256;
                            else if (memory[IP]==0xDC+i) BL = regis8[i+4]/256;
                            //AH-BH
                            else if (memory[IP]==0xE4+i) AH = regis8[i+4];
                            else if (memory[IP]==0xEC+i) CH = regis8[i+4];
                            else if (memory[IP]==0xF4+i) DH = regis8[i+4];
                            else if (memory[IP]==0xFC+i) BH = regis8[i+4];
                        }
                        IP++;
                        updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;

                    case 0x8B:
                        for (unsigned char i = 0; i < 8; i++){  //note: AH*256
                            if (memory[IP]==0xC0+i) AX = regis[i];
                            else if (memory[IP]==0xC8+i) CX = regis[i];
                            else if (memory[IP]==0xD0+i) DX = regis[i];
                            else if (memory[IP]==0xD8+i) BX = regis[i];
                            else if (memory[IP]==0xE0+i) SP = regis[i];
                            else if (memory[IP]==0xE8+i) BP = regis[i];
                            else if (memory[IP]==0xF0+i) SI = regis[i];
                            else if (memory[IP]==0xF8+i) DI = regis[i];
                        }
                        IP++;
                        updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
     
                //ADD
                //adding 16-bit register to 16-bit register
                case 0x03:
                    for (unsigned char i = 0; i < 8; i++){  //note: AH*256
                        T = regis[i];
                        if (memory[IP]==0xC0+i) {
                            AX += regis[i];
                             zero_flag(AX,ZF);
                             sign_flag (AX,SF);
                             carry_flag(AX,CF,T,1); 
                             } 
                        else if (memory[IP]==0xC8+i){ 
                            CX += regis[i];
                            zero_flag(CX,ZF);
                            sign_flag (CX,SF);
                            carry_flag(CX,CF,T,1);}
                        else if (memory[IP]==0xD0+i){
                             DX += regis[i];
                            zero_flag(DX,ZF);
                            sign_flag (DX,SF);
                            carry_flag(DX,CF,T,1);                            
                            }
                        else if (memory[IP]==0xD8+i){ 
                            BX += regis[i];
                            zero_flag(BX,ZF);
                            sign_flag (BX,SF);
                            carry_flag(BX,CF,T,1);                       
                            }
                        else if (memory[IP]==0xE0+i){
                            SP += regis[i];
                            zero_flag(SP,ZF);
                            sign_flag (SP,SF);
                            carry_flag(SP,CF,T,1);                         
                            }
                        else if (memory[IP]==0xE8+i){
                            BP += regis[i];
                            zero_flag(BP,ZF);
                            sign_flag (BP,SF);
                            carry_flag(BP,CF,T,1); 
                            }
                        else if (memory[IP]==0xF0+i) {
                            SI += regis[i];
                            zero_flag(SI,ZF);
                            sign_flag (SI,SF);
                            carry_flag(SI,CF,T,1);
                            } 
                        else if (memory[IP]==0xF8+i){
                            DI += regis[i];
                            zero_flag(DI,ZF);
                            sign_flag (DI,SF);
                            carry_flag(DI,CF,T,1);
                            }
                    }
                    IP++;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                break;
                //adding 8-bit register to 8-bit register
                case 0x02:
                        for (unsigned char i = 0; i < 4; i++){  
                        T = regis8[i];
                        //adding lower 8bits
                            //AL-BL
                            if (memory[IP]==0xC0+i) {
                                AL += regis8[i];
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);
                                carry_flag(AL,CF,T,1);
                                } 
                            else if (memory[IP]==0xC8+i){
                                CL += regis8[i];
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);
                                carry_flag(CL,CF,T,1);
                            }                            
                            else if (memory[IP]==0xD0+i){
                                DL += regis8[i];
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);
                                carry_flag(DL,CF,T,1);
                            }
                            else if (memory[IP]==0xD8+i){
                                 BL += regis8[i];
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);
                                carry_flag(BL,CF,T,1);
                            }
                            //AH-BH
                            else if (memory[IP]==0xE0+i){
                                AH += regis8[i]*256;
                                zero_flag(AH,ZF); 
                                sign_flag (AH,SF);
                                carry_flag(AH,CF,T,1);
                            }
                            else if (memory[IP]==0xE8+i) {
                                CH += regis8[i]*256;
                                zero_flag(CH,ZF); 
                                sign_flag (CH,SF);
                                carry_flag(CH,CF,T,1);
                                }
                            else if (memory[IP]==0xF0+i) {
                                DH += regis8[i]*256;
                                zero_flag(DH,ZF); 
                                sign_flag (DH,SF);
                                carry_flag(DH,CF,T,1);
                                }                                
                            else if (memory[IP]==0xF8+i) {
                                BH += regis8[i]*256;
                                zero_flag(BH,ZF); 
                                sign_flag (BH,SF);
                                carry_flag(BH,CF,T,1);
                                }
                            //adding higher 8bits   like: ADD AL,AH
                            //AL-BL
                            else if (memory[IP]==0xC4+i) {
                                AL += regis8[i+4]/256;
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);
                                carry_flag(AL,CF,T,1);
                                }
                            else if (memory[IP]==0xCC+i) {
                                CL += regis8[i+4]/256;
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);
                                carry_flag(CL,CF,T,1);
                                }
                            else if (memory[IP]==0xD4+i) {
                                DL += regis8[i+4]/256;
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);
                                carry_flag(DL,CF,T,1);
                                }
                            else if (memory[IP]==0xDC+i) {
                                BL += regis8[i+4]/256;
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);
                                carry_flag(BL,CF,T,1);
                                }
                            //AH-BH
                            else if (memory[IP]==0xE4+i) {
                                AH += regis8[i+4];
                                 zero_flag(AH,ZF); 
                                sign_flag (AH,SF);
                                carry_flag(AH,CF,T,1);
                            }
                            else if (memory[IP]==0xEC+i) {
                                CH += regis8[i+4];
                                zero_flag(CH,ZF); 
                                sign_flag (CH,SF);
                                carry_flag(CH,CF,T,1);
                                }
                            else if (memory[IP]==0xF4+i) {
                                DH += regis8[i+4];
                                zero_flag(DH,ZF); 
                                sign_flag (DH,SF);
                                carry_flag(DH,CF,T,1);
                            }
                            else if (memory[IP]==0xFC+i){
                                BH += regis8[i+4];
                                zero_flag(BH,ZF); 
                                sign_flag (BH,SF);
                                carry_flag(BH,CF,T,1);
                                }
                        }
                        IP++;
                        updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
            //adding constant to 8-bit register
                case 0x04: //adding constant to register AL
                    T = regis8[0];
                    AL += memory [IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AL,ZF);
                    sign_flag (AL,SF);
                    carry_flag(AL,CF,T,1);
                break;

            //adding constant to 16-bit register
                case 0x05: // adding a constant to register AX
                    T = regis[0];
                    AX += memory[IP++] + memory[IP++]*256;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AX,ZF);
                    sign_flag (AX,SF);
                    carry_flag(AX,CF,T,1);
                break;

            //SUBTRACT
                //subtracting 16-bit register from a 16-bit register
                case 0x2B:
                    for (unsigned char i = 0; i < 8; i++){ 
                        T = regis[i];
                        if (memory[IP]==0xC0+i){ 
                            AX -= regis[i];
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                        else if (memory[IP]==0xC8+i) {
                            CX -= regis[i];
                            zero_flag(CX,ZF);
                            sign_flag (CX,SF);
                            carry_flag(CX,CF,T,0); 
                        }
                        else if (memory[IP]==0xD0+i) {
                            DX -= regis[i];
                            zero_flag(DX,ZF);
                            sign_flag (DX,SF);
                            carry_flag(DX,CF,T,0); 
                        }
                        else if (memory[IP]==0xD8+i) {
                            BX -= regis[i];
                            zero_flag(BX,ZF);
                            sign_flag (BX,SF);
                            carry_flag(BX,CF,T,0); 
                        }
                        else if (memory[IP]==0xE0+i) {
                            SP -= regis[i];
                            zero_flag(SP,ZF);
                            sign_flag (SP,SF);
                            carry_flag(SP,CF,T,0); 
                        }
                        else if (memory[IP]==0xE8+i) {
                            BP -= regis[i];
                            zero_flag(BP,ZF);
                            sign_flag (BP,SF);
                            carry_flag(BP,CF,T,0); 
                        }
                        else if (memory[IP]==0xF0+i) {
                            SI -= regis[i];
                            zero_flag(SI,ZF);
                            sign_flag (SI,SF);
                            carry_flag(SI,CF,T,0); 
                        }
                        else if (memory[IP]==0xF8+i) {
                            DI -= regis[i];
                            zero_flag(DI,ZF);
                            sign_flag (DI,SF);
                            carry_flag(DI,CF,T,0); 
                        }
                    }
                    IP++;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                break;
                //subtracting 8-bit register from 8-bit register
                case 0x2A:
                        for (unsigned char i = 0; i < 4; i++){ 
                            //AL-BL
                            if (memory[IP]==0xC0+i) {
                                AL -= regis8[i];
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);
                                carry_flag(AL,CF,T,0); 
                        }
                            else if (memory[IP]==0xC8+i) {
                                CL -= regis8[i];
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);
                                carry_flag(CL,CF,T,0); 
                        }
                            else if (memory[IP]==0xD0+i) {
                                DL -= regis8[i];
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);
                                carry_flag(DL,CF,T,0); 
                        }
                            else if (memory[IP]==0xD8+i) {
                                BL -= regis8[i];
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);
                                carry_flag(BL,CF,T,0); 
                        }
                                

                            //AH-BH
                            else if (memory[IP]==0xE0+i) {
                                AH -= regis8[i]*256;
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);
                                carry_flag(AH,CF,T,0); 
                        }
                            else if (memory[IP]==0xE8+i) {
                                CH -= regis8[i]*256;
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);
                                carry_flag(CH,CF,T,0); 
                        }
                            else if (memory[IP]==0xF0+i) {
                                DH -= regis8[i]*256;
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);
                                carry_flag(DH,CF,T,0); 
                        }
                            else if (memory[IP]==0xF8+i) {
                                BH -= regis8[i]*256;
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);
                                carry_flag(BH,CF,T,0); 
                        }
                            //AL-BL
                            else if (memory[IP]==0xC4+i) {
                                AL -= regis8[i+4]/256;
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);
                                carry_flag(AL,CF,T,0); 
                        }
                            else if (memory[IP]==0xCC+i){
                                CL -= regis8[i+4]/256;
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);
                                carry_flag(CL,CF,T,0); 
                        }
                            else if (memory[IP]==0xD4+i) {
                                DL -= regis8[i+4]/256;
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);
                                carry_flag(DL,CF,T,0); 
                        }
                            else if (memory[IP]==0xDC+i) {
                                BL -= regis8[i+4]/256;
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);
                                carry_flag(BL,CF,T,0); 
                        }
                            //AH-BH
                            else if (memory[IP]==0xE4+i) {
                                AH -= regis8[i+4];
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);
                                carry_flag(AH,CF,T,0); 
                        }
                            else if (memory[IP]==0xEC+i) {
                                CH -= regis8[i+4];
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);
                                carry_flag(CH,CF,T,0); 
                        }
                            else if (memory[IP]==0xF4+i) {
                                DH -= regis8[i+4];
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);
                                carry_flag(DH,CF,T,0); 
                        }
                            else if (memory[IP]==0xFC+i) {
                                BH -= regis8[i+4];
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);
                                carry_flag(BH,CF,T,0); 
                        }
                        }
                        IP++;
                        updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
            //subtracting constant from 8-bit register
                case 0x2C: //subtracting constant to register AL
                    T = regis8[0];
                    AL -= memory [IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AL,ZF);
                    sign_flag (AL,SF);
                    carry_flag(AL,CF,T,0);
                break;

            //subtracting constant from 16-bit register
                case 0x2D: // subtracting a constant to register AX
                     T = regis[0];
                    regis[0]-= memory[IP++] + memory[IP++]*256;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AX,ZF);
                    sign_flag (AX,SF);
                    carry_flag(AX,CF,T,0);
                break;

            //OR ---> 16-bit register ---> NO CARRY FLAG
                case 0x0B:
                    for (unsigned char i = 0; i < 8; i++){ 
                        if (memory[IP]==0xC0+i) {
                            AX |= regis[i];
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);}
                        else if (memory[IP]==0xC8+i) {
                            CX |= regis[i];
                            zero_flag(CX,ZF);
                            sign_flag (CX,SF);}
                        else if (memory[IP]==0xD0+i) {
                            DX |= regis[i];
                            zero_flag(DX,ZF);
                            sign_flag (DX,SF);}
                        else if (memory[IP]==0xD8+i) {
                            BX |= regis[i];
                            zero_flag(BX,ZF);
                            sign_flag (BX,SF);}                           
                        else if (memory[IP]==0xE0+i) {
                            SP |= regis[i];
                            zero_flag(SP,ZF);
                            sign_flag (SP,SF);}
                        else if (memory[IP]==0xE8+i) {
                            BP |= regis[i];
                            zero_flag(BP,ZF);
                            sign_flag (BP,SF);}
                        else if (memory[IP]==0xF0+i) {
                            SI |= regis[i];
                            zero_flag(SI,ZF);
                            sign_flag (SI,SF);}
                        else if (memory[IP]==0xF8+i) {
                            DI |= regis[i];
                            zero_flag(DI,ZF);
                            sign_flag (DI,SF);}
                    }
                    IP++;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                break;
                //OR 8-bit register to 8-bit register
                case 0x0A:
                        for (unsigned char i = 0; i < 4; i++){ 
                            //AL-BL
                            if (memory[IP]==0xC0+i) {
                                AL |= regis8[i];
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);}
                            else if (memory[IP]==0xC8+i) {
                                CL |= regis8[i];
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);}
                            else if (memory[IP]==0xD0+i) { 
                                DL |= regis8[i];
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);}
                            else if (memory[IP]==0xD8+i) {
                                BL |= regis8[i];
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);}
                            //AH|BH
                            else if (memory[IP]==0xE0+i) {
                                AH |= regis8[i]*256;
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);}
                            else if (memory[IP]==0xE8+i) {
                                CH |= regis8[i]*256;
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);}
                            else if (memory[IP]==0xF0+i) {
                                DH |= regis8[i]*256;
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);}
                            else if (memory[IP]==0xF8+i) {
                                BH |= regis8[i]*256;
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);}
                            //AL|BL
                            else if (memory[IP]==0xC4+i) {
                                AL |= regis8[i+4]/256;
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);}
                            else if (memory[IP]==0xCC+i) {
                                CL |= regis8[i+4]/256;
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);}
                            else if (memory[IP]==0xD4+i) {
                                DL |= regis8[i+4]/256;
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);}
                            else if (memory[IP]==0xDC+i) {
                                BL |= regis8[i+4]/256;
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);}
                            //AH|BH
                            else if (memory[IP]==0xE4+i) {
                                AH |= regis8[i+4];
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);}
                            else if (memory[IP]==0xEC+i) {
                                CH |= regis8[i+4];
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);}
                            else if (memory[IP]==0xF4+i) {
                                DH |= regis8[i+4];
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);}
                            else if (memory[IP]==0xFC+i) {
                                BH |= regis8[i+4];
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);}
                        }
                        IP++;
                        updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
            //OR constant to 8-bit register
                case 0x0C: //OR constant to register AL
                    AL |= memory [IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AL,ZF);
                    sign_flag (AL,SF);;
                break;

            //OR constant to 16-bit register
                case 0x0D: // OR a constant to register AX
                    regis[0] |= memory[IP++] + memory[IP++]*256;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AX,ZF);
                    sign_flag (AX,SF);;
                break;

            //XOR
                //XOR 16-bit register from a 16-bit register
                case 0x33:
                    for (unsigned char i = 0; i < 8; i++){  //note: AH*256
                        if (memory[IP]==0xC0+i) {
                            AX ^= regis[i];
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);}
                        else if (memory[IP]==0xC8+i) {
                            CX ^= regis[i];
                            zero_flag(CX,ZF);
                            sign_flag (CX,SF);}
                        else if (memory[IP]==0xD0+i) {
                            DX ^= regis[i];
                            zero_flag(DX,ZF);
                            sign_flag (DX,SF);}
                        else if (memory[IP]==0xD8+i) {
                            BX ^= regis[i];
                            zero_flag(BX,ZF);
                            sign_flag (BX,SF);}
                        else if (memory[IP]==0xE0+i) {
                            SP ^= regis[i];
                            zero_flag(SP,ZF);
                            sign_flag (SP,SF);}
                        else if (memory[IP]==0xE8+i){
                             BP ^= regis[i];
                            zero_flag(BP,ZF);
                            sign_flag (BP,SF);}
                        else if (memory[IP]==0xF0+i) {
                            SI ^= regis[i];
                            zero_flag(SI,ZF);
                            sign_flag (SI,SF);}
                        else if (memory[IP]==0xF8+i) {
                            DI ^= regis[i];
                            zero_flag(DI,ZF);
                            sign_flag (DI,SF);}
                    }
                    IP++;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                break;
                //XOR 8-bit register to 8-bit register
                case 0x32:
                        for (unsigned char i = 0; i < 4; i++){ 
                            //AL-BL
                            if (memory[IP]==0xC0+i) {
                                AL ^= regis8[i];
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);}
                            else if (memory[IP]==0xC8+i) {
                                CL ^= regis8[i];
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);}
                            else if (memory[IP]==0xD0+i) {
                                DL ^= regis8[i];
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);}
                            else if (memory[IP]==0xD8+i) {
                                BL ^= regis8[i];
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);}
                            //AH^BH
                            else if (memory[IP]==0xE0+i) {
                                AH ^= regis8[i]*256;
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);}
                            else if (memory[IP]==0xE8+i) {
                                CH ^= regis8[i]*256;
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);}
                            else if (memory[IP]==0xF0+i) {
                                DH ^= regis8[i]*256;
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);}
                            else if (memory[IP]==0xF8+i) {
                                BH ^= regis8[i]*256;
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);}
                            //AL^BL
                            else if (memory[IP]==0xC4+i) {
                                AL ^= regis8[i+4]/256;
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);}
                            else if (memory[IP]==0xCC+i) {
                                CL ^= regis8[i+4]/256;
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);}
                            else if (memory[IP]==0xD4+i) {
                                DL ^= regis8[i+4]/256;
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);}
                            else if (memory[IP]==0xDC+i) {
                                BL ^= regis8[i+4]/256;
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);}
                            //AH^BH
                            else if (memory[IP]==0xE4+i) {
                                AH ^= regis8[i+4];
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);}
                            else if (memory[IP]==0xEC+i) {
                                CH ^= regis8[i+4];
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);}
                            else if (memory[IP]==0xF4+i) {
                                DH ^= regis8[i+4];
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);}
                            else if (memory[IP]==0xFC+i) {
                                BH ^= regis8[i+4];
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);}
                        }
                        IP++;
                        updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
            //XOR constant to 8-bit register
                case 0x34: //XOR constant to register AL
                    AL ^= memory [IP++];
                    updateRegis16(AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AL,ZF);
                    sign_flag (AL,SF);
                break;

            //XOR constant to 16-bit register
                case 0x35: //XOR a constant to register AX
                    regis[0]^= memory[IP++] + memory[IP++]*256;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AX,ZF);
                    sign_flag (AX,SF);
                break;

            //AND
                //AND 16-bit register from a 16-bit register
                case 0x23:
                    for (unsigned char i = 0; i < 8; i++){ 
                        if (memory[IP]==0xC0+i) {
                            AX &= regis[i];
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);}
                        else if (memory[IP]==0xC8+i){
                            CX &= regis[i];
                            zero_flag(CX,ZF);
                            sign_flag (CX,SF);}
                        else if (memory[IP]==0xD0+i) {
                            DX &= regis[i];
                            zero_flag(DX,ZF);
                            sign_flag (DX,SF);}
                        else if (memory[IP]==0xD8+i) {
                            BX &= regis[i];
                            zero_flag(BX,ZF);
                            sign_flag (BX,SF);}
                        else if (memory[IP]==0xE0+i) {
                            SP &= regis[i];
                            zero_flag(SP,ZF);
                            sign_flag (SP,SF);}
                        else if (memory[IP]==0xE8+i) {
                            BP &= regis[i];
                            zero_flag(BP,ZF);
                            sign_flag (BP,SF);}
                        else if (memory[IP]==0xF0+i) {
                            SI &= regis[i];
                            zero_flag(SI,ZF);
                            sign_flag (SI,SF);}
                        else if (memory[IP]==0xF8+i) {
                            DI &= regis[i];
                            zero_flag(DI,ZF);
                            sign_flag (DI,SF);}
                    }
                    IP++;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                break;
                //AND 8-bit register to 8-bit register
                case 0x22:
                        for (unsigned char i = 0; i < 4; i++){ 
                            //AL-BL
                            if (memory[IP]==0xC0+i) {
                                AL &= regis8[i];
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);}
                            else if (memory[IP]==0xC8+i) {
                                CL &= regis8[i];
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);}
                            else if (memory[IP]==0xD0+i) {
                                DL &= regis8[i];
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);}
                            else if (memory[IP]==0xD8+i) {
                                BL &= regis8[i];
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);}
                            //AH&BH
                            else if (memory[IP]==0xE0+i) {
                                AH &= regis8[i]*256;
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);}
                            else if (memory[IP]==0xE8+i) {
                                CH &= regis8[i]*256;
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);}
                            else if (memory[IP]==0xF0+i) {
                                DH &= regis8[i]*256;
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);}
                            else if (memory[IP]==0xF8+i) {
                                BH &= regis8[i]*256;
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);}
                            //AL&BL
                            else if (memory[IP]==0xC4+i) {
                                AL &= regis8[i+4]/256;
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);}
                            else if (memory[IP]==0xCC+i) {
                                CL &= regis8[i+4]/256;
                                zero_flag(CL,ZF);
                                sign_flag (CL,SF);}
                            else if (memory[IP]==0xD4+i) {
                                DL &= regis8[i+4]/256;
                                zero_flag(DL,ZF);
                                sign_flag (DL,SF);}
                            else if (memory[IP]==0xDC+i) {
                                BL &= regis8[i+4]/256;
                                zero_flag(BL,ZF);
                                sign_flag (BL,SF);}
                            //AH&BH
                            else if (memory[IP]==0xE4+i) {
                                AH &= regis8[i+4];
                                zero_flag(AH,ZF);
                                sign_flag (AH,SF);}
                            else if (memory[IP]==0xEC+i) {
                                CH &= regis8[i+4];
                                zero_flag(CH,ZF);
                                sign_flag (CH,SF);}
                            else if (memory[IP]==0xF4+i) {
                                DH &= regis8[i+4];
                                zero_flag(DH,ZF);
                                sign_flag (DH,SF);}
                            else if (memory[IP]==0xFC+i) {
                                BH &= regis8[i+4];
                                zero_flag(BH,ZF);
                                sign_flag (BH,SF);}
                        }
                        IP++;
                        updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    break;
            //AND constant to 8-bit register
                case 0x24: //AND constant to register AL
                    AL &= memory [IP++];
                    updateRegis16 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                    zero_flag(AL,ZF);
                    sign_flag (AL,SF);
                break;

            //AND constant to 16-bit register
                case 0x25: //AND a constant to register AX
                 regis[0]&= memory[IP++] + memory[IP++]*256;
                 updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                 zero_flag(AX,ZF);
                 sign_flag (AX,SF);
                break;
            case 0x80:
            //for lower registers
                    for (unsigned char i = 0; i < 4; i++){  
                        if (memory[IP]==0xC0+i) {
                                regis8[i] += memory[IP+1]; //add to L
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);
                                carry_flag(AL,CF,T,0); 
                        }
                        else if (memory[IP]==0xE8+i) {
                                regis8[i] -= memory[IP+1]; //sub to L
                                zero_flag(AL,ZF);
                                sign_flag (AL,SF);
                                carry_flag(AL,CF,T,0); 
                        }
                        else if (memory[IP]==0xC8+i) {
                            regis8[i] |= memory[IP+1]; //OR
                            zero_flag(AL,ZF);
                            sign_flag (AL,SF);}
                        else if (memory[IP]==0xF0+i) {
                            regis8[i] ^= memory[IP+1]; //XOR
                            zero_flag(AL,ZF);
                            sign_flag (AL,SF);}
                        else if (memory[IP]==0xE0+i) {
                            regis8[i] &= memory[IP+1]; //AND
                            zero_flag(AL,ZF);
                            sign_flag (AL,SF);}
                    }
            //for higher registers
                    for (unsigned char i = 4; i < 8; i++){
                        if (memory[IP]==0xC0+i) {
                            regis8[i] += memory[IP+1]*256; //add to H
                            zero_flag(AH,ZF);
                            sign_flag (AH,SF);
                            carry_flag(AH,CF,T,0); 
                        }
                        else if (memory[IP]==0xE8+i) {
                            regis8[i] -= memory[IP+1]*256; //sub to H
                            zero_flag(AH,ZF);
                            sign_flag (AH,SF);
                            carry_flag(AH,CF,T,0); 
                        }
                        else if (memory[IP]==0xC8+i) {
                            regis8[i] |= memory[IP+1]*256; //OR
                            sign_flag (AH,SF);
                            carry_flag(AH,CF,T,0); 
                        }
                        else if (memory[IP]==0xF0+i) {
                            regis8[i] ^= memory[IP+1]*256; //XOR
                            sign_flag (AH,SF);
                            carry_flag(AH,CF,T,0); 
                        }
                        else if (memory[IP]==0xE0+i) {
                            regis8[i] &= memory[IP+1]*256; //AND
                            sign_flag (AH,SF);
                            carry_flag(AH,CF,T,0); 
                        }
                    }                    
                    IP+=2;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
            break;

                case 0x81: //adding 16-bit constant to 16-bit register, ADD AX,AX
                    for(unsigned char i = 0; i <8;i++){
                         if (memory[IP]==0xC0+i) {
                             regis[i] += memory[IP+1] + memory[IP+2]*256; //add to X
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                         else if (memory[IP]==0xE8+i) {
                             regis[i] -= memory[IP+1] + memory[IP+2]*256; //subtract from X
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                         else if (memory[IP]==0xC8+i) {
                             regis[i] |= memory[IP+1] + memory[IP+2]*256; //OR
                             sign_flag (AX,SF);
                             carry_flag(AX,CF,T,0); 
                        }
                         else if (memory[IP]==0xF0+i) {
                             regis[i] ^= memory[IP+1] + memory[IP+2]*256; //XOR
                             sign_flag (AX,SF);
                             carry_flag(AX,CF,T,0); 
                        }
                         else if (memory[IP]==0xE0+i) {
                             regis[i] &= memory[IP+1] + memory[IP+2]*256; //AND
                             sign_flag (AX,SF);
                             carry_flag(AX,CF,T,0); 
                        }
                        //and, or, xor
                    }
                    IP+=3;
                    updateRegis8 (AX,CX,DX,BX,AL,AH,CL,CH,DL,DH,BL,BH);
                break;

                case 0x83: 
                    for(unsigned char i = 0; i <8; i++){
                        //adding a 8-bit constant to a 16-bit register
                        if(memory[IP] == 0xC0 + i) {
                            regis[i]+= memory[++IP]; //ADD
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                        else if(memory[IP] == 0xE8 +i) {
                            regis[i]-= memory[++IP]; //SUB
                            zero_flag(AX,ZF);
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                        else if(memory[IP] == 0xC8 + i) {
                            regis[i] |= memory[++IP]; //OR
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                        else if(memory[IP] == 0xF0 +i) {
                            regis[i] ^= memory[++IP]; //XOR
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                        else if(memory[IP] == 0xE0 + i) {
                            regis[i] &= memory[++IP]; //AND
                            sign_flag (AX,SF);
                            carry_flag(AX,CF,T,0); 
                        }
                    }

                //cout<< DX +AX + CX <<endl; //when uncommented it should give you 7 when you run the MOVSUB.COM file
                    IP++;
                break;

               //LOOPS
               case 0xE2:
                        if (CX != 0 && ZF == 0) {
                            signed char loopcount = memory[IP++]; 
                            CX--;
                            IP += loopcount; //go back to pointer of loop     
                        }
                        else IP++;
                    break;
                //LOOPZ (checks if ZF is 0)
                case 0xE1:
                  if (CX != 0 && ZF == 1){
                    signed char loopcount = memory[IP++];
                    CX--;
                    if(CX==0) break;
                    else IP +=loopcount; 
                  }
                    else IP++;
                break;
                //LOOPNZ (checks if ZF is 1)
                case 0xE0:
                  if (CX != 0 && ZF == 0){ 
                    signed char loopcount = memory[IP++];
                    CX--;
                    if(CX==0) break;
                    else IP +=loopcount; 
                  }
                    else IP++;
                break;

                //PUSH : pushing values to stack
                case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
                    stack[SP++] = regis[byte&7];
                break;

                //POP : restoring the top of stack into the register
                case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
                    regis[byte&7] = stack[--SP];
                break;

                //MULTIPLY and DIVIDE ---> register to register
                case 0xF6: //multiply to AL ----> multiply 8-bit to AL
                for (int i = 0; i<8;i++){
                    if(memory[IP] == 0xE0 + i)AX = AL* regis8[memory[IP]]; //MULTIPLY
                    else if(memory[IP] == 0xF0 + i) { //DIVIDE
                        AL = AX /regis8[memory[IP]]; //quotient
                        AH = AX % regis8[memory[IP]]; //remainder
                    }
                    if (AX > 255)CF = 1;
                    else CF = 0;
                }
                IP++;
                break;
                case 0xF7: //multiply to AX
                for (int i = 0; i <8; i++){
                    if(memory[IP] == 0xE0 + i){
                    unsigned long var = AX * regis[memory[IP]]; //use this to store extra in DX, var can take more than 65535
                    AX *= regis[memory[IP]];     
                    DX = var/65536;  //similar to AH  
                    }   
                    else if(memory[IP] == 0xF0 + i){
                    AX /= regis[memory[IP]]; //quotient
                    DX = AX % regis[memory[IP]];//remainder
                    } 
                    if (AX > 255)CF = 1;
                    else CF = 0; 
                }
                IP++;
                break;     

                case 0x90: 
                    break;
                case 0xCD:
                    if(memory[IP] != 0x20)
                    {
                         cout<<"ERROR!!";
                    }
                    else
                    {
                        exit(0);
                    } 
                    break;
            }
            // uncomment this to see the output
            cout<<"-----START----"<<endl;
            cout<<"AX ="<< AX<<endl;
            cout<<"CX ="<< CX<<endl;
            cout<<"DX ="<< DX<<endl;
            cout<<"BX ="<< BX<<endl;
            cout<<"SP ="<< SP<<endl;
            cout<<"BP ="<< BP<<endl;
            cout<<"SI ="<< SI<<endl;
            cout<<"DI ="<< DI<<endl;           
            cout<<"CF ="<< CF<<endl;
            cout<<"SF ="<< SF<<endl;
            cout<<"ZF ="<< ZF<<endl;          

        }
    }

   return 0; 
}
