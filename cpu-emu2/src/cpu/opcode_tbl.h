/*
  opcode_tbl.h 
  init: 16.8.2015

  GPL
*/

struct opcode_entry {
  int opcode;
  int illegal;
  char *name;
  char *adrmode;
  int bytes;
  int cycles;
  
  void (* const op_fct)(struct _6510_cpu* cpu, char memory[][9] );  //const pointer to operator function
  char *mnemonic;
  char *adrmode_descr;
  char *flags;
  char *info;
  char *action;
  int (* const adrfct)(struct _6510_cpu* cpu, char memory[][9] );  //const pointer to addressing function
  char *logic;
};
extern struct opcode_entry opcode_tbl[];
