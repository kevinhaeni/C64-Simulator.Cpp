/*
  initial: 31.1.2015
  implement fetch execute cycle, interrupts
  16.8.2015: implement step
*/

#include "flags.h"
//#include "opcode_tbl.h"
#include "decode.h"

extern struct _vic* vic;

int pc2int(struct _6510_cpu*cpu){
  return  (conv_bitstr2int(cpu->pch,0,7)<<8 ) | ( conv_bitstr2int(cpu->pcl,0,7));
}


int fetch( struct _6510_cpu* cpu, char memory[][9]  ) {
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);
  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->dbr, cpu->ir);
  
  inc_pc(cpu);  
  return conv_bitstr2int(cpu->ir,0,7);
}

int exec( struct _6510_cpu* cpu, char memory[][9] ) {
  int op_code = conv_bitstr2int(cpu->ir,0,7);
  struct opcode_entry* entryp = operation_lookup(op_code);
  void (*fct)(struct _6510_cpu* cpu, char memory[][9] );
 
 fct = entryp->op_fct;
  if(fct){
    (*fct)(cpu, memory);
    draw_bitmap_memory(cpu, vic, memory);
    cpu->cycles += entryp->cycles;
    if(cpu->cycles %1000 <10)
      draw_bitmap_memory(cpu, vic, memory);
    
  } else {
    return -1;
  }
  return 1;
}

void fetch_exec_step( struct _6510_cpu* cpu, char memory[][9] ) {
  int opcode;
  opcode = fetch(cpu, memory);
  exec(cpu, memory);
}

void fetch_exec_cycle( struct _6510_cpu* cpu, char memory[][9]  ) {
  int opcode;
  while(1){
    fetch_exec_step(cpu, memory);
  }
}



void reset_cpu(struct _6510_cpu* cpu){
  char null[9] = "00000000";
  cp_byte(null,cpu->pcl);
  cp_byte(null,cpu->pch);
  cp_byte(null,cpu->regx);
  cp_byte(null,cpu->regy);
  cp_byte(null,cpu->rega);
  cp_byte("00110000",cpu->flags);
  cp_byte("11111110",cpu->sp);
  cp_byte(null,cpu->abrl);
  cp_byte(null,cpu->abrh);
  cp_byte(null,cpu->dbr);
  cp_byte(null,cpu->ir);
  cp_byte(null,cpu->pcl);
  cp_byte(null,cpu->pcl);
  cp_byte(null,cpu->pcl);
  cpu->cycles=0;
}

