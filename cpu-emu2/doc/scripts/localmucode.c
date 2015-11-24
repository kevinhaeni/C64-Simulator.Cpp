/*
  name:    BRK
  opcode:  0x00 
  ex:      BRK 
  eadr:    implizit
  flags:   NVBDIZC :--1-1--
  bytes:   1 
  desc:    interrupt 
  form1:   (S)-=:PC,P PC:=($FFFE) 
  form2:   Stack <- PC, PC <- ($fffe) 
  form3:   *       PC = PC + 1 
           *       bPoke(SP,PC.h) 
           *       SP = SP - 1 
           *       bPoke(SP,PC.l) 
           *       SP = SP - 1 
           *       bPoke(SP, (P|$10) ) 
           *       SP = SP - 1 
           *       l = bPeek($FFFE) 
           *       h = bPeek($FFFF)<<8 
           *       PC = h|l              
           *    
          
*/

void cpu_6502_BRK_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_BRK(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x01 
  ex:      ORA ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x05 
  ex:      ORA $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ASL
  opcode:  0x06 
  ex:      ASL $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}


/*
  name:    PHP
  opcode:  0x08 
  ex:      PHP 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    push processor status (SR) 
  form1:   (S)-:=P 
  form2:   Stack <- (P)               
  form3:   *       bPoke(SP,P) 
           *       SP = SP - 1  
           *    
          
*/

void cpu_6502_PHP_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_PHP(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x09 
  ex:      ORA #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ASL
  opcode:  0x0A 
  ex:      ASL 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x0D 
  ex:      ORA $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ASL
  opcode:  0x0E 
  ex:      ASL $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}


/*
  name:    BPL
  opcode:  0x10 
  ex:      BPL $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on plus (negative clear) 
  form1:   branch on N=0 
  form2:   if N=0, PC = PC + offset   
  form3:   *       if (P.N == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BPL_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BPL(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x11 
  ex:      ORA ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x15 
  ex:      ORA $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ASL
  opcode:  0x16 
  ex:      ASL $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}


/*
  name:    CLC
  opcode:  0x18 
  ex:      CLC 
  eadr:    implizit
  flags:   NVBDIZC :------0
  bytes:   1 
  desc:    clear carry 
  form1:   C:=0 
  form2:   C <- 0                     
  form3:   *       P.C = 0  
           *    
          
*/

void cpu_6502_CLC_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_CLC(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x19 
  ex:      ORA $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ORA
  opcode:  0x1D 
  ex:      ORA $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}


/*
  name:    ASL
  opcode:  0x1E 
  ex:      ASL $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}


/*
  name:    JSR
  opcode:  0x20 
  ex:      JSR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    jump subroutine 
  form1:   (S)-:=PC PC:={adr} 
  form2:   Stack <- PC, PC <- Address 
  form3:   *       t = PC - 1 
           *       bPoke(SP,t.h) 
           *       SP = SP - 1 
           *       bPoke(SP,t.l) 
           *       SP = SP - 1 
           *       PC = $A5B6     
           *    
          
*/

void cpu_6502_JSR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_JSR(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x21 
  ex:      AND ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    BIT
  opcode:  0x24 
  ex:      BIT $AB
  eadr:    zero page
  flags:   NVBDIZC :**---*-
  bytes:   2 
  desc:    bit test 
  form1:   N:=b7 V:=b6 Z:=A\&{adr} 
  form2:   Z <- ~(A /\ M) N<-M7 V<-M6 
  form3:   *       t = A & M 
           *       P.N = t.7 
           *       P.V = t.6 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_BIT_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_BIT(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x25 
  ex:      AND $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    ROL
  opcode:  0x26 
  ex:      ROL $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}


/*
  name:    PLP
  opcode:  0x28 
  ex:      PLP 
  eadr:    implizit
  flags:   NVBDIZC :**-****
  bytes:   1 
  desc:    pull processor status (SR) 
  form1:   P:=+(S) 
  form2:   A <- (Stack)               
  form3:   *       SP = SP + 1 
           *       P = bPeek(SP)   
           *    
          
*/

void cpu_6502_PLP_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_PLP(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x29 
  ex:      AND #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    ROL
  opcode:  0x2A 
  ex:      ROL 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}


/*
  name:    BIT
  opcode:  0x2C 
  ex:      BIT $ABCD
  eadr:    absolute
  flags:   NVBDIZC :**---*-
  bytes:   3 
  desc:    bit test 
  form1:   N:=b7 V:=b6 Z:=A\&{adr} 
  form2:   Z <- ~(A /\ M) N<-M7 V<-M6 
  form3:   *       t = A & M 
           *       P.N = t.7 
           *       P.V = t.6 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_BIT_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_BIT(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x2D 
  ex:      AND $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    ROL
  opcode:  0x2E 
  ex:      ROL $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}


/*
  name:    BMI
  opcode:  0x30 
  ex:      BMI $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on minus (negative set) 
  form1:   branch on N=1 
  form2:   if N=1, PC = PC + offset   
  form3:   *       if (P.N == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BMI_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BMI(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x31 
  ex:      AND ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x35 
  ex:      AND $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    ROL
  opcode:  0x36 
  ex:      ROL $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}


/*
  name:    SEC
  opcode:  0x38 
  ex:      SEC 
  eadr:    implizit
  flags:   NVBDIZC :------1
  bytes:   1 
  desc:    set carry 
  form1:   C:=1 
  form2:   C <- 1                     
  form3:   *       P.C = 1  
           *    
          
*/

void cpu_6502_SEC_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_SEC(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x39 
  ex:      AND $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    AND
  opcode:  0x3D 
  ex:      AND $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}


/*
  name:    ROL
  opcode:  0x3E 
  ex:      ROL $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}


/*
  name:    RTI
  opcode:  0x40 
  ex:      RTI 
  eadr:    implizit
  flags:   NVBDIZC :**-****
  bytes:   1 
  desc:    return from interrupt 
  form1:   P,PC:=+(S) 
  form2:   P <- (Stack), PC <-(Stack) 
  form3:   *       SP = SP - 1 
           *       P = bPeek(SP) 
           *       SP = SP - 1 
           *       l = bPeek(SP) 
           *       SP = SP - 1 
           *       h = bPeek(SP)<<8 
           *       PC = h|l          
           *    
          
*/

void cpu_6502_RTI_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_RTI(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x41 
  ex:      EOR ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x45 
  ex:      EOR $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    LSR
  opcode:  0x46 
  ex:      LSR $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}


/*
  name:    PHA
  opcode:  0x48 
  ex:      PHA 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    push accumulator 
  form1:   (S)-:=A 
  form2:   Stack <- (A)               
  form3:   *       bPoke(SP,A) 
           *       SP = SP - 1  
           *    
          
*/

void cpu_6502_PHA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_PHA(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x49 
  ex:      EOR #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    LSR
  opcode:  0x4A 
  ex:      LSR 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}


/*
  name:    JMP
  opcode:  0x4C 
  ex:      JMP $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    jump 
  form1:   PC:={adr} 
  form2:   PC <- Address              
  form3:   *       PC = M  
           *    
          
*/

void cpu_6502_JMP_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_JMP(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x4D 
  ex:      EOR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    LSR
  opcode:  0x4E 
  ex:      LSR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}


/*
  name:    BVC
  opcode:  0x50 
  ex:      BVC $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on overflow clear 
  form1:   branch on V=0 
  form2:   if V=0, PC = PC + offset   
  form3:   *       if (P.V == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BVC_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BVC(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x51 
  ex:      EOR ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x55 
  ex:      EOR $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    LSR
  opcode:  0x56 
  ex:      LSR $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}


/*
  name:    CLI
  opcode:  0x58 
  ex:      CLI 
  eadr:    implizit
  flags:   NVBDIZC :----0--
  bytes:   1 
  desc:    clear interrupt disable 
  form1:   I:=0 
  form2:   I <- 0                     
  form3:   *       P.I = 0  
           *    
          
*/

void cpu_6502_CLI_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_CLI(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x59 
  ex:      EOR $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    EOR
  opcode:  0x5D 
  ex:      EOR $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}


/*
  name:    LSR
  opcode:  0x5E 
  ex:      LSR $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}


/*
  name:    RTS
  opcode:  0x60 
  ex:      RTS 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    return from subroutine 
  form1:   PC:=+(S) 
  form2:   PC <- (Stack)              
  form3:   *       SP = SP + 1 
           *       l = bPeek(SP) 
           *       SP = SP + 1 
           *       h = bPeek(SP)<<8 
           *       PC = (h|l) +1     
           *    
          
*/

void cpu_6502_RTS_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_RTS(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x61 
  ex:      ADC ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x65 
  ex:      ADC $AB
  eadr:    zero page
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF   
           *    
          
*/

void cpu_6502_ADC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ROR
  opcode:  0x66 
  ex:      ROR $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}


/*
  name:    PLA
  opcode:  0x68 
  ex:      PLA 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    pull accumulator 
  form1:   A:=+(S) 
  form2:   A <- (Stack)               
  form3:   *       SP = SP + 1 
           *       A = bPeek(SP) 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_PLA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_PLA(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x69 
  ex:      ADC #$AB    
  eadr:    immediate
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ROR
  opcode:  0x6A 
  ex:      ROR 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}


/*
  name:    JMP
  opcode:  0x6C 
  ex:      JMP $ABCD
  eadr:    absolute indirect
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    jump 
  form1:   PC:={adr} 
  form2:   PC <- Address              
  form3:   *       PC = M  
           *    
          
*/

void cpu_6502_JMP_ind(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = ind_adr_mode(cpu, mem);
  do_JMP(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x6D 
  ex:      ADC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ROR
  opcode:  0x6E 
  ex:      ROR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}


/*
  name:    BVS
  opcode:  0x70 
  ex:      BVS $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on overflow set 
  form1:   branch on V=1 
  form2:   if V=1, PC = PC + offset   
  form3:   *       if (P.V == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BVS_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BVS(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x71 
  ex:      ADC ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x75 
  ex:      ADC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ROR
  opcode:  0x76 
  ex:      ROR $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}


/*
  name:    SEI
  opcode:  0x78 
  ex:      SEI 
  eadr:    implizit
  flags:   NVBDIZC :----1--
  bytes:   1 
  desc:    set interrupt disable 
  form1:   I:=1 
  form2:   I <- 1                     
  form3:   *       P.I = 1  
           *    
          
*/

void cpu_6502_SEI_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_SEI(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x79 
  ex:      ADC $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ADC
  opcode:  0x7D 
  ex:      ADC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}


/*
  name:    ROR
  opcode:  0x7E 
  ex:      ROR $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x81 
  ex:      STA ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    STY
  opcode:  0x84 
  ex:      STY $AB
  eadr:    zero page
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store Y 
  form1:   {adr}:=Y 
  form2:   M <- (Y)                   
  form3:   *       M = Y   
           *    
          
*/

void cpu_6502_STY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_STY(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x85 
  ex:      STA $AB
  eadr:    zero page
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    STX
  opcode:  0x86 
  ex:      STX $AB
  eadr:    zero page
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store X 
  form1:   {adr}:=X 
  form2:   M <- (X)                   
  form3:   *       M = X   
           *    
          
*/

void cpu_6502_STX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_STX(cpu, mem[eadr]);


}


/*
  name:    DEY
  opcode:  0x88 
  ex:      DEY 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    decrement Y 
  form1:   Y:=Y-1 
  form2:   Y <- (Y) - 1               
  form3:   *       Y = Y - 1 
           *       P.Z = (Y==0) ? 1:0 
           *       P.N = Y.7           
           *    
          
*/

void cpu_6502_DEY_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_DEY(cpu, mem[eadr]);


}


/*
  name:    TXA
  opcode:  0x8A 
  ex:      TXA 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer X to accumulator 
  form1:   A:=X 
  form2:   A <- (X)                   
  form3:   *       A = X 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_TXA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_TXA(cpu, mem[eadr]);


}


/*
  name:    STY
  opcode:  0x8C 
  ex:      STY $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store Y 
  form1:   {adr}:=Y 
  form2:   M <- (Y)                   
  form3:   *       M = Y   
           *    
          
*/

void cpu_6502_STY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_STY(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x8D 
  ex:      STA $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    STX
  opcode:  0x8E 
  ex:      STX $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store X 
  form1:   {adr}:=X 
  form2:   M <- (X)                   
  form3:   *       M = X   
           *    
          
*/

void cpu_6502_STX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_STX(cpu, mem[eadr]);


}


/*
  name:    BCC
  opcode:  0x90 
  ex:      BCC $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on carry clear 
  form1:   branch on C=0 
  form2:   if C=0, PC = PC + offset   
  form3:   *       if (P.C == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BCC_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BCC(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x91 
  ex:      STA ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    STY
  opcode:  0x94 
  ex:      STY $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store Y 
  form1:   {adr}:=Y 
  form2:   M <- (Y)                   
  form3:   *       M = Y   
           *    
          
*/

void cpu_6502_STY_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_STY(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x95 
  ex:      STA $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    STX
  opcode:  0x96 
  ex:      STX $AB,X
  eadr:    zero page with index Y
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store X 
  form1:   {adr}:=X 
  form2:   M <- (X)                   
  form3:   *       M = X   
           *    
          
*/

void cpu_6502_STX_zpy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpy_adr_mode(cpu, mem);
  do_STX(cpu, mem[eadr]);


}


/*
  name:    TYA
  opcode:  0x98 
  ex:      TYA 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer Y to accumulator 
  form1:   A:=Y 
  form2:   A <- (Y)                   
  form3:  
*/

void cpu_6502_TYA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_TYA(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x99 
  ex:      STA $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    TXS
  opcode:  0x9A 
  ex:      TXS 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    transfer X to stack pointer 
  form1:   S:=X 
  form2:   S <- (X)                   
  form3:   *       SP = X  
           *    
          
*/

void cpu_6502_TXS_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_TXS(cpu, mem[eadr]);


}


/*
  name:    STA
  opcode:  0x9D 
  ex:      STA $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}


/*
  name:    LDY
  opcode:  0xA0 
  ex:      LDY #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xA1 
  ex:      LDA ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    LDX
  opcode:  0xA2 
  ex:      LDX #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}


/*
  name:    LDY
  opcode:  0xA4 
  ex:      LDY $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xA5 
  ex:      LDA $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    LDX
  opcode:  0xA6 
  ex:      LDX $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}


/*
  name:    TAY
  opcode:  0xA8 
  ex:      TAY 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer accumulator to Y 
  form1:   Y:=A 
  form2:   Y <- (A)                   
  form3:   *       Y = A 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0  
           *    
          
*/

void cpu_6502_TAY_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_TAY(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xA9 
  ex:      LDA #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    TAX
  opcode:  0xAA 
  ex:      TAX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer accumulator to X 
  form1:   X:=A 
  form2:   X <- (A)                   
  form3:   *       X = A 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0  
           *    
          
*/

void cpu_6502_TAX_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_TAX(cpu, mem[eadr]);


}


/*
  name:    LDY
  opcode:  0xAC 
  ex:      LDY $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xAD 
  ex:      LDA $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    LDX
  opcode:  0xAE 
  ex:      LDX $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}


/*
  name:    BCS
  opcode:  0xB0 
  ex:      BCS $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on carry set 
  form1:   branch on C=1 
  form2:   if C=1, PC = PC + offset   
  form3:   *       if (P.C == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BCS_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BCS(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xB1 
  ex:      LDA ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    LDY
  opcode:  0xB4 
  ex:      LDY $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xB5 
  ex:      LDA $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    LDX
  opcode:  0xB6 
  ex:      LDX $AB,X
  eadr:    zero page with index Y
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_zpy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpy_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}


/*
  name:    CLV
  opcode:  0xB8 
  ex:      CLV 
  eadr:    implizit
  flags:   NVBDIZC :-0-----
  bytes:   1 
  desc:    clear overflow 
  form1:   V:=0 
  form2:   V <- 0                     
  form3:   *       P.V = 0  
           *    
          
*/

void cpu_6502_CLV_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_CLV(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xB9 
  ex:      LDA $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    TSX
  opcode:  0xBA 
  ex:      TSX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer stack pointer to X 
  form1:   X:=S 
  form2:   X <- (S)                   
  form3:   *       X = SP 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0  
           *    
          
*/

void cpu_6502_TSX_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_TSX(cpu, mem[eadr]);


}


/*
  name:    LDY
  opcode:  0xBC 
  ex:      LDY $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}


/*
  name:    LDA
  opcode:  0xBD 
  ex:      LDA $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}


/*
  name:    LDX
  opcode:  0xBE 
  ex:      LDX $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}


/*
  name:    CPY
  opcode:  0xC0 
  ex:      CPY #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with Y 
  form1:   Y-{adr} 
  form2:   (Y - M) -> NZC             
  form3:   *       t = Y - M 
           *       P.N = t.7 
           *       P.C = (Y>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPY_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xC1 
  ex:      CMP ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    CPY
  opcode:  0xC4 
  ex:      CPY $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with Y 
  form1:   Y-{adr} 
  form2:   (Y - M) -> NZC             
  form3:   *       t = Y - M 
           *       P.N = t.7 
           *       P.C = (Y>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xC5 
  ex:      CMP $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    DEC
  opcode:  0xC6 
  ex:      DEC $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}


/*
  name:    INY
  opcode:  0xC8 
  ex:      INY 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    increment Y 
  form1:   Y:=Y+1 
  form2:   Y <- (Y) + 1               
  form3:   *       Y = Y + 1 
           *       P.Z = (Y==0) ? 1:0 
           *       P.N = Y.7           
           *    
          
*/

void cpu_6502_INY_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_INY(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xC9 
  ex:      CMP #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    DEX
  opcode:  0xCA 
  ex:      DEX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    decrement X 
  form1:   X:=X-1 
  form2:   X <- (X) - 1               
  form3:   *       X = X - 1 
           *       P.Z = (X==0) ? 1:0 
           *       P.N = X.7           
           *    
          
*/

void cpu_6502_DEX_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_DEX(cpu, mem[eadr]);


}


/*
  name:    CPY
  opcode:  0xCC 
  ex:      CPY $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare with Y 
  form1:   Y-{adr} 
  form2:   (Y - M) -> NZC             
  form3:   *       t = Y - M 
           *       P.N = t.7 
           *       P.C = (Y>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xCD 
  ex:      CMP $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    DEC
  opcode:  0xCE 
  ex:      DEC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}


/*
  name:    BNE
  opcode:  0xD0 
  ex:      BNE $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on not equal (zero clear) 
  form1:   branch on Z=0 
  form2:   if Z=0, PC = PC + offset   
  form3:   *       if (P.Z == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BNE_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BNE(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xD1 
  ex:      CMP ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xD5 
  ex:      CMP $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    DEC
  opcode:  0xD6 
  ex:      DEC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}


/*
  name:    CLD
  opcode:  0xD8 
  ex:      CLD 
  eadr:    implizit
  flags:   NVBDIZC :---0---
  bytes:   1 
  desc:    clear decimal 
  form1:   D:=0 
  form2:   D <- 0                     
  form3:   *       P.D = 0  
           *    
          
*/

void cpu_6502_CLD_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_CLD(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xD9 
  ex:      CMP $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    CMP
  opcode:  0xDD 
  ex:      CMP $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}


/*
  name:    DEC
  opcode:  0xDE 
  ex:      DEC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}


/*
  name:    CPX
  opcode:  0xE0 
  ex:      CPX #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with X 
  form1:   X-{adr} 
  form2:   (X - M) -> NZC             
  form3:   *       t = X - M 
           *       P.N = t.7 
           *       P.C = (X>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPX_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xE1 
  ex:      SBC ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    CPX
  opcode:  0xE4 
  ex:      CPX $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with X 
  form1:   X-{adr} 
  form2:   (X - M) -> NZC             
  form3:   *       t = X - M 
           *       P.N = t.7 
           *       P.C = (X>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xE5 
  ex:      SBC $AB
  eadr:    zero page
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    INC
  opcode:  0xE6 
  ex:      INC $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}


/*
  name:    INX
  opcode:  0xE8 
  ex:      INX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    increment X 
  form1:   X:=X+1 
  form2:   X <- (X) +1                
  form3:   *       X = X + 1 
           *       P.Z = (X==0) ? 1:0 
           *       P.N = X.7           
           *    
          
*/

void cpu_6502_INX_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_INX(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xE9 
  ex:      SBC #$AB    
  eadr:    immediate
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    NOP
  opcode:  0xEA 
  ex:      NOP 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    no operation 
  form1:     
  form2:   [no operation]             
  form3:   *       ~none~  
           *    
          
*/

void cpu_6502_NOP_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_NOP(cpu, mem[eadr]);


}


/*
  name:    CPX
  opcode:  0xEC 
  ex:      CPX $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare with X 
  form1:   X-{adr} 
  form2:   (X - M) -> NZC             
  form3:   *       t = X - M 
           *       P.N = t.7 
           *       P.C = (X>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xED 
  ex:      SBC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    INC
  opcode:  0xEE 
  ex:      INC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}


/*
  name:    BEQ
  opcode:  0xF0 
  ex:      BEQ $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on equal (zero set) 
  form1:   branch on Z=1 
  form2:   if Z=1, PC = PC + offset   
  form3:   *       if (P.Z == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BEQ_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  do_BEQ(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xF1 
  ex:      SBC ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xF5 
  ex:      SBC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    INC
  opcode:  0xF6 
  ex:      INC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}


/*
  name:    SED
  opcode:  0xF8 
  ex:      SED 
  eadr:    implizit
  flags:   NVBDIZC :---1---
  bytes:   1 
  desc:    set decimal 
  form1:   D:=1 
  form2:   D <- 1                     
  form3:   *       P.D = 1  
           *    
          
*/

void cpu_6502_SED_imp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imp_adr_mode(cpu, mem);
  do_SED(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xF9 
  ex:      SBC $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    SBC
  opcode:  0xFD 
  ex:      SBC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}


/*
  name:    INC
  opcode:  0xFE 
  ex:      INC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}


