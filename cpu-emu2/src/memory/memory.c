/*
  memory.c
  init: bo@1.2.2015
  implements memory: mem_mao, memory_access

*/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <memory.h>

#include "register.h"

#define LORAM 0
#define HIRAM 1
#define CHAREN 2


void init_memory(char memory[][9] ) {
  int i,j;
  for(i=0;i<MEMORY_SIZE;i++){
    for(j=0;j<8;j++){
      memory[i][j] = '0';
    }
    memory[i][j] = 0;
  }
}

char* ram(int adr, char memory[][9]){
  return memory[adr];
}
char* basic_ROM(int adr, char memory[][9]){
  return memory[adr];
}

char* kernel_ROM(int adr, char memory[][9]){
  return memory[adr];
}

char* char_ROM(int adr, char memory[][9]){
  return memory[adr];
}

char* IO_ram(int adr, char memory[][9]){
  return memory[adr];
}

void access_memory( struct _6510_cpu* cpu, char memory[][9]) {
  int adr = word2int(cpu->abrh, cpu->abrl);
  
  if(adr==0){ 
  } else  {
    if(adr==1){
    } else {
      if(adr>0xA000 || adr<0xBFFF){ // Basic ROM
	if(memory[1][LORAM] =='0'){
	  cp_byte(ram(adr, memory), cpu->dbr);  // OFF
	} else {
	  cp_byte(basic_ROM(adr, memory),cpu->dbr);  // ON
	} 
      }
    }
  }
  if(adr>0xD000 || adr<0xDFFF){ //  CHAR ROM
    if(memory[1][CHAREN] =='0'){
      cp_byte(char_ROM(adr, memory),cpu->dbr);  // ON
    } else {
      if(memory[1][0]=='0' && memory[1][1]=='0') {
	cp_byte(ram(adr, memory),cpu->dbr);  // OFF
      } else { 
	cp_byte(IO_ram(adr, memory),cpu->dbr);  // I/O memory
      }
	
    }
    if(adr>0xE000){    // kernel_ROM
      if(memory[1][HIRAM] =='0'){
	cp_byte(ram(adr, memory),cpu->dbr); // OFF
      } else {
	cp_byte(kernel_ROM(adr, memory),cpu->dbr); // ON      
      }
    }
  }
}


/*
  some helper function transforming int
  to f!*#@ing "10101010" format
*/
void int2register(int number, char* reg){
  reg[8]=0x0;
  int i;
  for(i=0; i<8;i++){
    if(number & 1<<i)
      reg[7-i]='1';
    else
      reg[7-i]='0';
  }
}

void read_memorydump(char *fname, char memory[][9]){
  char buffer_reg[8+1];
  int adr, content;
  FILE *stream;
  
  buffer_reg[8] = '\0';
  if(!strcmp("-", fname)){
    stream = stdin;
  } else if ((stream = fopen( fname,"r")) == NULL){
    printf("Can't open %s\n", fname);
    exit(1);
  }
  while(!feof(stream)){
    // format string is:
    // <any space, including 0> <hex address> <any space, including 0> <hex content>
    //DELME    fscanf(stream, " %i %8c ", &adr, buffer);
    fscanf(stream, " %4x %2x ", &adr, &content);
    if(adr>=MEMORY_SIZE){
      printf("Illegal Address %i\nSkipping line ..\n", adr);
    } else {
      //	  printf("%i: %i->%02x\n", i++, adr, content);
      int2register(content, buffer_reg);
      cp_byte(buffer_reg, memory[adr]);
    }
  }
}

void read_binary_memorydump(char *fname, char memory[][9], int start){

  unsigned char c;
  int adr = start;
  FILE *stream;
  

  if(!strcmp("-", fname)){
    stream = stdin;
  } else if ((stream = fopen( fname,"r")) == NULL){
    printf("Can't open %s\n", fname);
    exit(1);
  }
  while(!feof(stream)){
    // format string is:
    // <any space, including 0> <hex address> <any space, including 0> <hex content>
    //DELME    fscanf(stream, " %i %8c ", &adr, buffer);
    if( 1 != fscanf(stream, "%c", &c)){
      break;
    }
    if(adr<0 || adr>=MEMORY_SIZE-1){
      printf("Illegal Address %i\nSkipping line ..\n", adr);
    } else {
      //	  printf("%i: %i->%02x\n", i++, adr, content);
      int2register(c, memory[adr++]);
    }
  }
}
