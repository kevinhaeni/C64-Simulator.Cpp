#!/bin/bash
SOURCE=../opcode_table_bigABC
TARGET=../../src/cpu/mu-code2.c


#opcode_table_bigABC
#0x01 @ 0x2 @ 0x6 @ ORA @ izx @ ADRM_IZX @ 1 @ ORA @ 0x01 @ cpu_6502_ora_izx @ izx @ or with accumulator @ ORA X,ind @ A:=A or {adr} @ *----*- @ ORA @    A = A | M @    P.N = A.7 @    P.Z = (A==0) ? 1:0  @ 
#0x61 @ 0x2 @ 0x6 @ ADC @ izx @ ADRM_IZX @ 1 @ ADC @ 0x61 @ cpu_6502_adc_izx @ izx @ add with carry @ ADC X,ind @ A:=A+{adr} @ **---** @ ADC @    t = A + M + P.C @    P.V = (A.7!=t.7) ? 1:0 @    P.N = A.7 @    P.Z = (t==0) ? 1:0 @    IF (P.D) @      t = bcd(A) + bcd(M) + P.C @      P.C = (t>99) ? 1:0 @    ELSE @      P.C = (t>255) ? 1:0 @    A = t & 0xFF                 @ 

awk -F"@" -e 'BEGIN { while (getline < "../codebase-c64.txt") { formal[$4]=$6; clocks[$4] = $9; special_clocks[$4] = $11; } }
END { print "formal[\"02\"] = " formal["FF"]  } 
$4!~/CRASH/ && $4!~/\*../{ opcode = $1; oplength=$2; opcycles=$3; opname=$4; opadrmode=$5; opadrmodeid=$6; cname=$10; shorttxt=$12; formaltxt=$14; flags=$15; ; logictxt="" ;for(i=17; i<=NF; i++){ logictxt=logictxt" *   "$i"\n          "}
lines = i-17;
switch(opadrmode){
case " imm " : {
ea = "PC+1"
  adrmodetxt = "immediate"
  exampletxt = "#$AB    "
  codetxt = "imm_adr_mode(cpu, mem);"
  break;
}
case " ind " : {
ea = "mem[abs]"
  adrmodetxt = "absolute indirect"
  exampletxt = "$ABCD"
  codetxt = "ind_adr_mode(cpu, mem);"
  break;
}
case " abs " : {
ea = "mem[abs_high|abs_low]"
  adrmodetxt = "absolute"
  exampletxt = "$ABCD"
  codetxt = "abs_adr_mode(cpu, mem);"
  break;
}
case " abx " : {
ea = "abs+X";
  adrmodetxt = "absolut with index X"
  exampletxt = "$ABCD,X";
  codetxt = "abx_adr_mode(cpu, mem);"
  break;
}
case " aby " : {
ea = "abs+Y"
  adrmodetxt = "absolut with index Y"
  exampletxt = "$ABCD,Y"
  codetxt = "aby_adr_mode(cpu, mem);"
  break;
}
case " izx " : {
ea = "mem[zpadr+X|zpadr+X+1]"
  adrmodetxt = "indexed with indirection"
  exampletxt = "($A5,X)"
  codetxt = "izx_adr_mode(cpu, mem);"
  break;
}
case " izy " : {
ea = "mem[zpadr|zpadr+1]+Y"
  adrmodetxt = "indirection with index"
  exampletxt = "($A5),Y"
  codetxt = "izy_adr_mode(cpu, mem);"
  break;
}
case " imp " : {
ea = "implizit"
  adrmodetxt = "implizit"
  exampletxt = "";
  codetxt = "";
  break;
}
case " rel " : {
ea = "pc + offset + 1"
  adrmodetxt = "relativ"
  exampletxt = "$AB";
  codetxt = "rel_adr_mode(cpu, mem);"
  break;
}
case " zp " : {
ea = "zpadr"
  adrmodetxt = "zero page"
  exampletxt = "$AB"
  codetxt = "zp_adr_mode(cpu, mem);"
  break;
}
case " zpx " : {
ea = "zpadr + X"
  adrmodetxt = "zero page with index X"
  exampletxt = "$AB,X"
  codetxt = "zpx_adr_mode(cpu, mem);"
  break;
}
case " zpy " : {
ea = "zpadr + Y"
  adrmodetxt = "zero page with index Y"
  exampletxt = "$AB,X"
  codetxt = "zpy_adr_mode(cpu, mem);"
  break;
}
default: {
 ea = ""
  adrmodetxt = "nix found"
  break;
}
}

#gsub("-","\\textendash",flags);
gsub(" ","",flags);
gsub(" ", "", opname)
gsub(" ", "", opadrmode)
gsub("\\&", "\\\\&", formaltxt)
flagstxt= "NVBDIZC :"flags;
# mu-code.c
printf("/*\n  name:    %s\n  opcode:  %s\n  ex:      %s %s\n  eadr:    %s\n  flags:   %s\n  bytes:   %s\n  desc:   %s\n  form1:  %s\n  form2:   %s\n  form3:  %s\n*/\n\nvoid cpu_6502_%s_%s(struct _6510_cpu* cpu, char mem[][9]){\n  int eadr = %s_adr_mode(cpu, mem);\n  do_%s(cpu, mem[eadr]);\n\n\n}\n\n\n",
       opname, opcode, opname, exampletxt, adrmodetxt, flagstxt, substr(oplength,4), shorttxt, formaltxt, formal[substr(opcode,3,2)] , logictxt, opname, opadrmode, opadrmode, opname, shorttxt, formaltxt) | "cat >./localmucode.c"
}
/ADC/ || /AND/ || /ASL/ || /CMP/ || /DEC/ || /EOR/ || /INC/ || /LSR/ || /ORA/ || /ROL/ || /ROR/ || /SBC/ { if( opadrmode~/abs/) { printf("/*\n  op:      %s\n  ex:      %02x %s %02x -> %02x and flags setting \n  flags:   %s\n           NVBDIZC\n  short:  %s\n  form1:  %s\n  form2:   %s\n  form3:  %s\n*/\n\nvoid alu_op_%s(char rega[], char regb[], char accumulator[], char flags[]){\n\n}\n\n\n",
       opname, 0x25, opname, 0xaf, 0xfe, flags, shorttxt, formaltxt, formal[substr(opcode,3,2)] , logictxt, opname, opadrmode, opadrmode, opname, shorttxt, formaltxt) | "cat >./local_alu.c"
}
#print "asldkasd " opadrmode " aaaaa"
}

' < $SOURCE  ###> $TARGET

