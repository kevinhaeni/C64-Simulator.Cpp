/*

  graphic-main.c
  bho1 - init 23.8.2015 

  Compile with  `sdl-config --cflags --libs`
  

  
*/
#include <string.h>
#include <stdio.h>

#include "register.h"
//#include "cpu.h"
//#include "flags.h"
//#include "alu.h"
//#include "decode.h"
#include "memory.h"
//#include "cpu-util.h"
#include "../sid/sidwrapper.h"
#include "../memorygrid/memorygridwrapper.h"

#include <signal.h>
static int keepParsing = 1;

#include <stdlib.h>

#include "graphic.h"

/* #include "bitmap.h" */
/* #include "chardev.h" */

#include "SDL.h"
#include "SDL_mouse.h"
//#include "SDL_events.h"
#include <SDL_events.h>

struct _vic* vic;

struct _6510_cpu cpu;
char memory[0x10000][9];



void intHandler(int dummy) {
  SDL_Quit();
  exit(-1);
  //  dummy++;
  // keepParsing = 1;
}
//
// write random into mem[0xfe]
// visual tutorial example screen
//
void rnd_update(struct _6510_cpu* cpu, struct _vic* vic, char memory[][9]){
  int r = rand() % 0x100;
  int2register(r,memory[0x00fe]);
}
  // used for random generation in visual-cpu simulator
  //   rnd_update(cpu, vic, memory);

/*
  main:
  options:

  --memory <memory-dump>
  Read file <memory-dump> and copy into memory

  --source <disassembler source>
  Load file <disassembler source> into source array
  --register <filename>
  copy content in <filename>  into corresponding register
*/
int main(int argc, char *argv[])
{
  
  signal(SIGINT, intHandler);
  
  init_memory(memory);
  
  reset_cpu(&cpu);
  struct _vic vicstruct;
  vic = &vicstruct;
  init_display(vic);
  
  // Init SDL
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
  TTF_Init();

  unsigned long SIDRefreshDelay = 0, MemoryGridRefreshDelay = 0;
  auto DELAY = 2;

  // Instantiate SID and memory grid
  struct MemoryGrid* memgrid = newMemoryGrid(&memory); 
  struct SID* sid = newSID(&memory, 1);


  // handling parameter --memory <dumpfile> 
  if(argc==3){
    if(!strcmp(argv[1], "--memory")){
      printf("Loading %s into memory\n", argv[2]);
      read_memorydump(argv[2], memory);
    }
  }
  // handling parameter --binary <dumpfile>  
  if(argc==4){
    if(!strcmp(argv[1], "--binary")){
      int adr =0;
      sscanf(argv[3], "%4x ", &adr);
    
      printf("Loading %s into memory at 0x%04x\n", argv[2], adr);
      // testvector
      read_binary_memorydump(argv[2], memory, adr);
      // cl-6502
      //      read_binary_memorydump(argv[2], memory, 0x0000);
    }
  }

  vic->videobase = 0x200;
  printf("Video memory base at %04x\n", vic->videobase);

  draw_bitmap_memory(&cpu, vic, memory); // draw_memory(memory+videobase);


  print_cpu_dis(&cpu,memory);
  while(1){
	// read input
	// parse input
	// do the stuff
    
    char *line = read_line();
    int z = parse_line(&cpu, memory, line);

    fetch(&cpu, memory);
    exec(&cpu, memory);

    print_cpu_dis(&cpu,memory);
    while(z){
      line = read_line();
      z = parse_line(&cpu, memory, line);
    }
    draw_bitmap_memory(&cpu, vic, memory);
   
  }

  printf("quit repl and cpu ..\n");
  SDL_Quit();
  exit(0);
  //  quit_monitor();
  return 0;
}
