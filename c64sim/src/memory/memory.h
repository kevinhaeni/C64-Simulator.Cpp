/*
  memory.h
*/

#include "cpu.h"
#define MEMORY_SIZE 65536 //Memory of 2^16 Memorycells
//#define MEMORY_SIZE 32   //Testing Purpose
#define MEMORY_CELL_SIZE 8 //8 character bits per cell
#define READ '1'
#define WRITE '0'
#define ADDR_WIDTH 16

char* ram(int adr, char memory[][9]);

char* basic_ROM(int adr, char memory[][9]);
char* kernel_ROM(int adr, char memory[][9]);
char* char_ROM(int adr, char memory[][9]);
char* IO_ram(int adr, char memory[][9]);

void init_memory(char memory[][9]);

void access_memory( struct _6510_cpu* cpu, char memory[][9]);


void read_dump(char *fname, char memory[][9]);

//void int2register(int number, char* reg);

void print_memory_page0(struct _6510_cpu* cpu, char memory[][9], int adr);


