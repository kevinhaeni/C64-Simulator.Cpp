/*
  cpu-main.c
  initial: 17.4.2015
  implements  cpu-main prompt

*/
#include <stdio.h>
#include "cpu.h"

struct _6510_cpu cpu;
char memory[0x10000][9];

/*
  main:
  options:
  --memory <memory-dump>
  Read file <memory-dump> and copy into memory
*/
int main(int argc, char* argv[]){

  init_memory(memory);
  
  reset_cpu(&cpu);
  
  // handling parameter --memory <dumpfile> 
  if(argc==3){
    if(!strcmp(argv[1], "--memory")){
      printf("Loading %s into memory\n", argv[2]);
      read_memorydump(argv[2], memory);
    }
  }
  
  
  /*
	
  fetch-exec cycle modes

  interactiv: scanf(%2x %2x %2x) and put the values
  into mem[pc] then start one cycle

  stepping: run one cycle, wait for any input, reiterate

  logging: write registers into file
  */
  print_cpu(&cpu);
  while(1){
	// read input
	// parse input
	// do the stuff
    
    fetch(&cpu, memory);
    exec(&cpu, memory);
    print_cpu(&cpu);
  }


  //  exit_monitor();

  return 1;
}
