/*
  decode.h 
  init: 16.8.2015

  GPL
*/

#include <stdlib.h>

#include "cpu.h"
#include "opcode_tbl.h"

// there are max 0xff operations
#define OPS 0xff 
//struct cpu_operator *operations;
//extern struct cpu_operator operations[];

#define ADRM_IMP 1
#define ADRM_IMM 2
#define ADRM_REL 3
#define ADRM_IZX 4
#define ADRM_IZY 5
#define ADRM_ZP 6
#define ADRM_ZPX 7
#define ADRM_ZPY 8
#define ADRM_ABS 9
#define ADRM_ABX 10
#define ADRM_ABY 11
#define ADRM_IND 12

struct opcode_entry* operation_lookup(int op_code);

extern struct opcode_entry opcode_tbl[];
