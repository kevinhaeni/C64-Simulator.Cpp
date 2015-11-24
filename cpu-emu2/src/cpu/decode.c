/*
  6502-decode.c
  bho1 30.1.2013

  GPL applies
*/

#include <stdio.h>
#include <search.h>

#include "register.h"

#include "cpu-util.h"
// #include "memory.h"
// #include "cpu.h"

//#include "6502-opcodes.h"
//#include "decode.h"

#include "mu-code.h"

#include "opcode_tbl.h"

#define OPCODES 256
/*
  array of structures containing the essentials:
  - text describing the command,
  - and int encoding the command,
  - function pointer executing the command

    
*/

struct opcode_entry* operation_lookup(int op_code){
  return &opcode_tbl[op_code];
}




/*

  Print information about cpu-op 

   void print_op (struct mu_code *c)
   {
   //char message[40];
   //snprintf (40, message, "Operator %s\n", c->name);
   }
   
*/


