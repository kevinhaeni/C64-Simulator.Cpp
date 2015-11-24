/* 
   alu.c 
   - 21.11.05/BHO1
   bho1 29.12.2006
   bho1 6.12.2007
   bho1 30.11.2007 - clean up
   bho1 24.11.2009 - assembler instruction

   GPL applies

   -->> YOUR FULL NAME HERE  <<--
*/

#include <stdio.h>
#include <string.h>

#include "alu.h"
#include "alu-opcodes.h"
#include "register.h"

int const max_mue_memory = 100;

extern char rega[REG_WIDTH+1];
extern char regb[REG_WIDTH+1];
extern char accumulator[REG_WIDTH+1];
extern char flags[9];

char mue_memory[100]= "100 Byte - this memory is at your disposal"; /*mue-memory */
char* m = mue_memory;
unsigned int c = 0; 	/*carry bit address*/
unsigned int s = 1;	/*sum bit address*/

void clearCarryflag(){
  flags[0] = '0';
}
void setCarryflag(){
  flags[0] = '1';
}
void clearSignflag(){
  flags[1] = '0';
}
void setSignflag(){
  flags[1] = '1';
}
void clearZeroflag(){
  flags[2] = '0';
}
void setZeroflag(){
  flags[2] = '1';
}
void clearOverflowflag(){
  flags[3] = '0';
}
void setOverflowflag(){
  flags[3] = '1';
}

/*
  testet ob alle bits im akkumulator auf null gesetzt sind.
  Falls ja wird 1 returniert, ansonsten 0
*/
int zero_test(){
  //your code here
}

/*
  Halfadder: addiert zwei character p,q und schreibt in 
  den Mue-memory das summen-bit und das carry-bit.
*/
void halfadder(char p, char q){
  //your code here
}

/* 
   void adder(char pbit, char qbit, char cbit)
   Adder oder auch Fulladder:
   Nimmt zwei character bits und ein carry-character-bit
   und schreibt das Resultat (summe, carry) in den Mue-speicher
*/
void adder(char pbit, char qbit, char cbit){
  //your code here
}

/*
  Invertieren der Character Bits im Register reg
*/
void one_complement(char* reg){
  //your code here

  // test with this line:
  //char* local-reg = reg;
  //and then in asm:
  // mov BL, [local-reg]

  __asm(
"		
     .intel_syntax noprefix ;switch to intel syntax

     push EAX  ;;need some register: save it on stack!!!
\n"
     "push EBX  \n"  //need some register: save it on stack!!!
	 
	 // from here on you are along
	 
	 "mov EAX,[ESP+16]\n"   // load EAX with address/position of MSB
	 // from string-parameter reg
"looping: \n"
	 
	 "mov BL, [EAX]\n"  //load BL with character in MEM[EAX]
	 "cmp BL, 0\n"      //test for end of string
	 "je finished\n"    //yes, so we have finished the work
	 "cmp BL, 0x30\n"   //test for ASCII(0x30) = '0'
	 "je set_to_one\n"  //Yes, so jump for setting '0' -> '1'
	 "mov BL, 0x30\n"   //No, so set BL to ASCII(0x30) = '0'
	 "mov BYTE PTR [eax], BL\n" //Putting char back into string
	 "add EAX, 1\n"     //next in line 
	 "jmp looping\n"    //Next in line

"set_to_one:\n"    //Label for setting BL to ASCII to '1'
	 "mov BL, 0x31\n"   //set BL to ASCII(0x31) = '1'
	 "mov BYTE PTR [eax], BL\n" //Putting char back into string
	 "add EAX, 1\n"     //next in line
	 "jmp looping\n"    //Next in line

"finished:\n"
	 "pop EBX \n"    //leaving without mess
	 "pop EAX \n"    //leaving without mess
	 ".att_syntax \n"     //switch back to AT&T syntax
	 
		);
  
}

/*
  Das zweier-Komplement des Registers reg wird in reg geschrieben
  reg := K2(reg)
*/
void two_complement(char* reg){
  //your code here
}

/*
  Die Werte in Register rega und Register regb werden addiert, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega + regb
*/
void op_add(){
  //your code here
}

/*
  Die Werte in Register rega und Register regb werden subtrahiert, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega - regb
*/
void op_sub(){
  //your code here
}

/*
  Die Werte in Register rega und Register regb werden logisch geANDet, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega AND regb
*/
void op_and(){
  //your code here
}

/*
  Die Werte in Register rega und Register regb werden logisch geORt, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega OR regb
*/
void op_or(){
  //your code here
}

/*
  Die Werte in Register rega und Register regb werden logisch geXORt, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega XOR regb
*/
void op_xor(){
  //your code here
}


/*
  Einer-Komplement von Register rega
  rega := not(rega)
*/
void op_not_a(){
  one_complement(rega);
}


/* Einer Komplement von Register regb */
void op_not_b(){
  //your code here
}


/*
  Negation von Register rega 
  rega := -rega
*/
void op_neg_a(){
  //your code here
}

/*
  Negation von Register regb 
  regb := -regb
*/
void op_neg_b(){
  //your code here
}

/*
  Reset des ALUs: Akumulator, beide Register, 
  der mue-memory und alle Flags werden auf Null gesetzt
*/
void op_reset(){
  int i;
  /* clear rega, regb, accumulator, flags */
  for(i=0; i<REG_WIDTH; i++){
    rega[i] = '0';
    regb[i] = '0';
    accumulator[i] = '0';
    flags[i] = '0';
  }
  /*clear mue_memory */
  for(i=0;i<max_mue_memory;i++)
    m[i] = '0';
}

/*
  The alu: takes an opcode and calls corresponding fuction
  mainly a switchboard
*/
void alu(unsigned int alu_opcode)
{
  switch ( alu_opcode ){
  case ALU_OP_ADD :
    op_add();
    break;
  case ALU_OP_SUB :
    op_sub();
    break;
  case ALU_OP_AND :
    op_and();
    break;
  case ALU_OP_OR:
    op_or();
    break;
  case ALU_OP_XOR :
    op_xor();
    break;
  case ALU_OP_NEG_A :
    op_neg_a();
    break;
  case ALU_OP_NEG_B :
    op_neg_b();
    break;
  case ALU_OP_NOT_A :
    op_not_a();
    break;
  case ALU_OP_NOT_B :
    op_not_b();
    break;
  case ALU_OP_RESET:
    op_reset();
    break;
  default:
    printf("ALU(%i): Invalide operation %i selected", alu_opcode, alu_opcode); 
  }	
}
