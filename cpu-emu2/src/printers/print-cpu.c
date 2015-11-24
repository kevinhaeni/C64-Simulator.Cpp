/*

  print-cpu.c
  bho 16.8.2015

  What?:
  prints cpu

  GPL applies

*/

#include <string.h>
#include <stdio.h>

#include "register.h"
#include "cpu.h"
#include "flags.h"

//#include "alu.h"
//#include "cpu.h"
//#include "memory.h"
//#include "cpu-util.h"

/* void printflags(char* flags){ */
/*   printf("flags: \t%s %c%c%c%c%c\n", flags, */
/* 	 ((getOverflowflag(flags)=='1')?'V':'-'), */
/* 	 (getCarryflag(flags)=='1'?'C':'-'), */
/* 	 (getSignflag(flags)=='1'?'N':'-'), */
/* 	 (getZeroflag(flags)=='1'?'Z':'-'), */
/* 	 (getBRKflag(flags)=='1'?'B':'-') */
/* 	 ); */
/* } */

void print_stack(struct _6510_cpu* cpu, char memory[][9]){
  int spi = conv_bitstr2int(cpu->sp,0,7);
  int i;
  for(i=spi-2;i<spi+3;i++){
    printf("mem[%02x'%02x] = \t   %02x\n", 1, i%256, conv_bitstr2int(memory[i%256+256],0,7));
  }
}

void print_memory_page0(struct _6510_cpu* cpu, char memory[][9], int adr){
 int i=0,j;
 char (*mem)[9] = memory[adr];
  for(j=0;j<5;j++){
    //    printf("%02x%02x ", 16*j+conv_bitstr2int(cpu->pch,0,7), 16*j+conv_bitstr2int(cpu->pcl,0,7));
    printf("%04x ", adr+j*16);
    for(i=0;i<16;i++){
      printf(" %02x ", conv_bitstr2int(mem++,0,7));
    }
    printf(" \n");
  }
}

void print_dis_memory(char memory[][9], int adr){
 int i=0,j;
 char (*mem)[9] = memory[adr];
 char dis[40];

 // int bytes =  dis_mem(memory, adr, dis) ? 1 : dis_mem(memory, adr, dis);
 int bytes = dis_mem(memory, adr, dis) ;

 for(j=0;j<5;j++){
    //    printf("%02x%02x ", 16*j+conv_bitstr2int(cpu->pch,0,7), 16*j+conv_bitstr2int(cpu->pcl,0,7));
    printf("%04x", adr);
    for(i=0;i<bytes;i++){
      printf(" %02x", conv_bitstr2int(memory[adr+i],0,7));
    }
    for(i=0;i<3-bytes;i++){
      printf("   ", conv_bitstr2int(memory[adr+i],0,7));
    }
    printf(" %s", dis);
    printf(" \n");
    adr = adr + bytes;
    bytes =  dis_mem(memory, adr, dis) ;
  }
}

void printm(struct _6510_cpu* cpu, char memory[][9]){
  int adr = 12 ; // word2int(cpu->pch, cpu->pcl);
  printf("\nmem[%02x'%02x] = \t   %02x\n", conv_bitstr2int(cpu->pch,0,7), conv_bitstr2int(cpu->pcl,0,7), conv_bitstr2int(memory[adr],0,7));

  print_stack(cpu, memory);
  printf("abrl:  \t\t   %02x\n",   conv_bitstr2int(cpu->abrl,0,7));
  printf("abrh:  \t\t   %02x\n",   conv_bitstr2int(cpu->abrh,0,7));
  printf("dbr:   \t\t   %02x\n",   conv_bitstr2int(cpu->dbr,0,7));
  printf("***********************************\n\n");
}
  

//  print_cpu
void print_cpu(struct _6510_cpu* cpu) {
  printf("pc   AC XR YR SP NV-BDIZC \n");
  printf("%02x%02x %02x %02x %02x %02x %c%c%c%c%c%c%c%c\n",
	 conv_bitstr2int(cpu->pch,0,7),
	 conv_bitstr2int(cpu->pcl,0,7),
	 conv_bitstr2int(cpu->rega,0,7),
	 conv_bitstr2int(cpu->regx,0,7),
	 conv_bitstr2int(cpu->regy,0,7),
	 conv_bitstr2int(cpu->sp,0,7),

	 cpu->flags[NFLAG],
	 cpu->flags[OFLAG],
	 '-',
	 cpu->flags[BFLAG],
	 cpu->flags[DFLAG],
	 cpu->flags[IFLAG],
	 cpu->flags[ZFLAG],
	 cpu->flags[CFLAG]
	 );
}

void print_cpu_dis(struct _6510_cpu* cpu, char memory[][9] ) {
  char dis_txt[] = "                                                                   \n";
  int pc = pc2int(cpu);
  dis(cpu, memory[pc], dis_txt);

  //  printf("pc    memory    dis        AC XR YR SP NV-BDIZC \n");
  printf("AC XR YR SP NV-BDIZC         \n");
  //  printf("%s", dis_txt);
  printf("%02x %02x %02x %02x %c%c%c%c%c%c%c%c %s\n",
	 //	 conv_bitstr2int(cpu->pch,0,7),
	 //	 conv_bitstr2int(cpu->pcl,0,7),
	 conv_bitstr2int(cpu->rega,0,7),
	 conv_bitstr2int(cpu->regx,0,7),
	 conv_bitstr2int(cpu->regy,0,7),
	 conv_bitstr2int(cpu->sp,0,7),

	 cpu->flags[NFLAG],
	 cpu->flags[OFLAG],
	 '-',
	 cpu->flags[BFLAG],
	 cpu->flags[DFLAG],
	 cpu->flags[IFLAG],
	 cpu->flags[ZFLAG],
    cpu->flags[CFLAG],
	 dis_txt
	 // conv_bitstr2int(ir,0,7),optr->name,optr->info
		 
	 );
}
