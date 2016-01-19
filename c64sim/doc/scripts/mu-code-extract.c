}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
void cpu_6502_illegal(struct _6510_cpu* cpu, char memory[][9] ){}
 


void do_ADC(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_ADD_WITH_CARRY, cpu->rega, mem, cpu->rega, cpu->flags);
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
}\\
void cpu_6502_BRK_imp(struct _6510_cpu* cpu, char mem[][9]){
  // Stack <- PC, PC <- ($fffe)
  int sp = sp2int(cpu);
  cp_register(cpu->pcl,mem[sp--]);
  cp_register(cpu->pch,mem[sp]);
  alu(ALU_OP_SUB, cpu->sp, "00000010", cpu->sp, 0);
  cp_register(mem[0xfffe],cpu->pcl);
  cp_register(mem[0xffff],cpu->pch);  


}\\
void cpu_6502_ORA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ORA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ASL_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}\\
void cpu_6502_PHP_imp(struct _6510_cpu* cpu, char mem[][9]){

  int sp = sp2int(cpu);
  cp_register(cpu->flags, mem[sp]);
  alu(ALU_OP_SUB, cpu->sp, "00000001", cpu->sp, 0);



}\\
void cpu_6502_ORA_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ASL_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_ASL(cpu, cpu->rega);


}\\
void cpu_6502_ORA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ASL_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}\\
void cpu_6502_BPL_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[SFLAG]=='0'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_ORA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ORA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ASL_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}\\
void cpu_6502_CLC_imp(struct _6510_cpu* cpu, char mem[][9]){

    cpu->flags[CFLAG] = '0';


}\\
void cpu_6502_ORA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ORA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);


}\\
void cpu_6502_ASL_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);


}\\
void cpu_6502_JSR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);

  char adr[] = "0000000000000000";
  int bits = 0x1;
  int i;
  for(i=0;i<16;i++){
    if(eadr>>i & bits){
      adr[15-i]='1';
    }
  }

  inc_pc(cpu);
  int sp = 0x100 | conv_bitstr2int(cpu->sp,0,7);
  cp_register(cpu->pch, mem[sp+1]);
  alu(ALU_OP_SUB, cpu->sp, "00000001", cpu->sp, 0);
  cp_register(cpu->pcl, mem[sp+1]);
  alu(ALU_OP_SUB, cpu->sp, "00000001", cpu->sp, 0);
  
  do_JSR(cpu, adr);

}\\
void cpu_6502_AND_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
  

}\\
void cpu_6502_BIT_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_BIT(cpu, mem[eadr]);


}\\
void cpu_6502_AND_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_ROL_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}\\
void cpu_6502_PLP_imp(struct _6510_cpu* cpu, char mem[][9]){
  // P <- (Stack)
  int sp = sp2int(cpu);

  cp_register(mem[++sp], cpu->flags);
  alu(ALU_OP_ADD, "00000001", cpu->sp, cpu->sp, 0);
}\\
void cpu_6502_AND_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_ROL_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_ROL(cpu, cpu->rega);


}\\
void cpu_6502_BIT_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_BIT(cpu, mem[eadr]);


}\\
void cpu_6502_AND_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_ROL_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}\\
void cpu_6502_BMI_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[SFLAG]=='1'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_AND_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_AND_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_ROL_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}\\
void cpu_6502_SEC_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[CFLAG] = '0';


}\\
void cpu_6502_AND_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_AND_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);


}\\
void cpu_6502_ROL_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);


}\\
void cpu_6502_RTI_imp(struct _6510_cpu* cpu, char mem[][9]){
// P <- (Stack), PC <-(Stack)

  int sp = sp2int(cpu);
  cp_register(mem[--sp], cpu->flags);
  cp_register(mem[--sp], cpu->pch);
  cp_register(mem[--sp], cpu->pcl);

  alu(ALU_OP_SUB, cpu->sp, "00000011", cpu->sp, 0);

}\\
void cpu_6502_EOR_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_EOR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_LSR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}\\
void cpu_6502_PHA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int sp = sp2int(cpu);
  cp_register(cpu->rega, mem[sp]);
  alu(ALU_OP_SUB, cpu->sp, "00000001", cpu->sp, 0);
}\\
void cpu_6502_EOR_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_LSR_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_LSR(cpu, cpu->rega);


}\\
void cpu_6502_JMP_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);

  char adr[] = "0000000000000000";
  int bits = 0x1;
  int i;
  for(i=0;i<16;i++){
    if(eadr>>i & bits){
      adr[15-i]='1';
    }
  }
  do_JMP(cpu, adr);
}\\
void cpu_6502_EOR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_LSR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}\\
void cpu_6502_BVC_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[VFLAG]=='0'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_EOR_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_EOR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_LSR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}\\
void cpu_6502_CLI_imp(struct _6510_cpu* cpu, char mem[][9]){
    cpu->flags[IFLAG] = '0';


}\\
void cpu_6502_EOR_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_EOR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);


}\\
void cpu_6502_LSR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);


}\\
void cpu_6502_RTS_imp(struct _6510_cpu* cpu, char mem[][9]){
  int sp = sp2int(cpu);
  
  cp_register(mem[sp], cpu->pcl);
  cp_register(mem[sp], cpu->pch);
  alu(ALU_OP_SUB, cpu->sp, "00000010", cpu->sp, 0);
  inc_pc(cpu);

}\\
void cpu_6502_ADC_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ADC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ROR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}\\
void cpu_6502_PLA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int sp = sp2int(cpu);

  cp_register(mem[++sp], cpu->rega);
  alu(ALU_OP_ADD, "00000001", cpu->sp, cpu->sp, 0);

}\\
void cpu_6502_ADC_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ROR_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_ROR(cpu, cpu->rega);


}\\
void cpu_6502_JMP_ind(struct _6510_cpu* cpu, char mem[][9]){
  int pc=pc2int(cpu);
  int iadr = ( conv_bitstr2int(mem[pc+1],0,7) << 8) | conv_bitstr2int(mem[pc],0,7) ;
  cp_register(mem[iadr] ,cpu->pcl) ;
  cp_register(mem[iadr++],cpu->pch) ;
  
}\\
void cpu_6502_ADC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ROR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}\\
void cpu_6502_BVS_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[VFLAG]=='1'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_ADC_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ADC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ROR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}\\
void cpu_6502_SEI_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[IFLAG] = '1';
}\\
void cpu_6502_ADC_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ADC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);


}\\
void cpu_6502_ROR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);


}\\
void cpu_6502_STA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}\\
void cpu_6502_STY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_STY(cpu, mem[eadr]);


}\\
void cpu_6502_STA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  //  int zp = conv_bitstr2int(mem[eadr],0,7);
  do_STA(cpu, mem[eadr]);


}\\
void cpu_6502_STX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_STX(cpu, mem[eadr]);


}\\
void cpu_6502_DEY_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_DEC(cpu, cpu->regy);


}\\
void cpu_6502_TXA_imp(struct _6510_cpu* cpu, char mem[][9]){  
  cp_register(cpu->regx,cpu->rega);
}\\
void cpu_6502_STY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_STY(cpu, mem[eadr]);


}\\
void cpu_6502_STA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}\\
void cpu_6502_STX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_STX(cpu, mem[eadr]);


}\\
void cpu_6502_BCC_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[CFLAG]=='0'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_STA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}\\
void cpu_6502_STY_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_STY(cpu, mem[eadr]);


}\\
void cpu_6502_STA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}\\
void cpu_6502_STX_zpy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpy_adr_mode(cpu, mem);
  do_STX(cpu, mem[eadr]);


}\\
void cpu_6502_TYA_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->regy,cpu->rega);
}\\
void cpu_6502_STA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);
}\\
void cpu_6502_TXS_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->regx,cpu->sp);
}\\
void cpu_6502_STA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_STA(cpu, mem[eadr]);


}\\
void cpu_6502_LDY_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}\\
void cpu_6502_LDA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_LDX_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}\\
void cpu_6502_LDY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}\\
void cpu_6502_LDA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_LDX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}\\
void cpu_6502_TAY_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->rega, cpu->regy);
}\\
void cpu_6502_LDA_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_TAX_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->rega, cpu->regx);
}\\
void cpu_6502_LDY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}\\
void cpu_6502_LDA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_LDX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}\\
void cpu_6502_BCS_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[CFLAG]=='1'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_LDA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_LDY_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}\\
void cpu_6502_LDA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_LDX_zpy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpy_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}\\
void cpu_6502_CLV_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[VFLAG] = '0';
}\\
void cpu_6502_LDA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_TSX_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->sp, cpu->regx);
}\\
void cpu_6502_LDY_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LDY(cpu, mem[eadr]);


}\\
void cpu_6502_LDA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LDA(cpu, mem[eadr]);


}\\
void cpu_6502_LDX_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_LDX(cpu, mem[eadr]);


}\\
void cpu_6502_CPY_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);


}\\
void cpu_6502_CMP_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
  

}\\
void cpu_6502_CPY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);


}\\
void cpu_6502_CMP_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_DEC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}\\
void cpu_6502_INY_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_INC(cpu, cpu->regy);


}\\
void cpu_6502_CMP_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_DEX_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_DEC(cpu, cpu->regx);


}\\
void cpu_6502_CPY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);


}\\
void cpu_6502_CMP_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_DEC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}\\
void cpu_6502_BNE_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  //   do_BNE(cpu, adr);
  if(cpu->flags[ZFLAG]=='0'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }




}\\
void cpu_6502_CMP_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_CMP_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_DEC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}\\
void cpu_6502_CLD_imp(struct _6510_cpu* cpu, char mem[][9]){
    cpu->flags[DFLAG] = '0';


}\\
void cpu_6502_CMP_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_CMP_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);


}\\
void cpu_6502_DEC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);


}\\
void cpu_6502_CPX_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);


}\\
void cpu_6502_SBC_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_CPX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);


}\\
void cpu_6502_SBC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_INC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}\\
void cpu_6502_INX_imp(struct _6510_cpu* cpu, char mem[][9]){

  do_INC(cpu, cpu->regx);

}\\
void cpu_6502_SBC_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_NOP_imp(struct _6510_cpu* cpu, char mem[][9]){
  
}\\
void cpu_6502_CPX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);


}\\
void cpu_6502_SBC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_INC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}\\
void cpu_6502_BEQ_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[ZFLAG]=='1'){
    cp_register(adr, cpu->pch);
    cp_register(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}\\
void cpu_6502_SBC_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_SBC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_INC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}\\
void cpu_6502_SED_imp(struct _6510_cpu* cpu, char mem[][9]){

  cpu->flags[DFLAG] = '0';


}\\
void cpu_6502_SBC_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_SBC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);


}\\
void cpu_6502_INC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);


}\\
