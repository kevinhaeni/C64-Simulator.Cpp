/***
 Automatic generated by gen-mucode.h.bash
***/
typedef struct _6510_cpu cpu;
void cpu_6502_BRK_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_izx(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_zp(cpu* cpu, char mem[][9] );
void cpu_6502_ASL_zp(cpu* cpu, char mem[][9] );
void cpu_6502_PHP_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_imm(cpu* cpu, char mem[][9] );
void cpu_6502_ASL_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_abs(cpu* cpu, char mem[][9] );
void cpu_6502_ASL_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BPL_rel(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_izy(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_ASL_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_CLC_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_aby(cpu* cpu, char mem[][9] );
void cpu_6502_ORA_abx(cpu* cpu, char mem[][9] );
void cpu_6502_ASL_abx(cpu* cpu, char mem[][9] );
void cpu_6502_JSR_abs(cpu* cpu, char mem[][9] );
void cpu_6502_AND_izx(cpu* cpu, char mem[][9] );
void cpu_6502_BIT_zp(cpu* cpu, char mem[][9] );
void cpu_6502_AND_zp(cpu* cpu, char mem[][9] );
void cpu_6502_ROL_zp(cpu* cpu, char mem[][9] );
void cpu_6502_PLP_imp(cpu* cpu, char mem[][9] );
void cpu_6502_AND_imm(cpu* cpu, char mem[][9] );
void cpu_6502_ROL_imp(cpu* cpu, char mem[][9] );
void cpu_6502_BIT_abs(cpu* cpu, char mem[][9] );
void cpu_6502_AND_abs(cpu* cpu, char mem[][9] );
void cpu_6502_ROL_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BMI_rel(cpu* cpu, char mem[][9] );
void cpu_6502_AND_izy(cpu* cpu, char mem[][9] );
void cpu_6502_AND_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_ROL_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_SEC_imp(cpu* cpu, char mem[][9] );
void cpu_6502_AND_aby(cpu* cpu, char mem[][9] );
void cpu_6502_AND_abx(cpu* cpu, char mem[][9] );
void cpu_6502_ROL_abx(cpu* cpu, char mem[][9] );
void cpu_6502_RTI_imp(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_izx(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_zp(cpu* cpu, char mem[][9] );
void cpu_6502_LSR_zp(cpu* cpu, char mem[][9] );
void cpu_6502_PHA_imp(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_imm(cpu* cpu, char mem[][9] );
void cpu_6502_LSR_imp(cpu* cpu, char mem[][9] );
void cpu_6502_JMP_abs(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_abs(cpu* cpu, char mem[][9] );
void cpu_6502_LSR_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BVC_rel(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_izy(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_LSR_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_CLI_imp(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_aby(cpu* cpu, char mem[][9] );
void cpu_6502_EOR_abx(cpu* cpu, char mem[][9] );
void cpu_6502_LSR_abx(cpu* cpu, char mem[][9] );
void cpu_6502_RTS_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_izx(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_zp(cpu* cpu, char mem[][9] );
void cpu_6502_ROR_zp(cpu* cpu, char mem[][9] );
void cpu_6502_PLA_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_imm(cpu* cpu, char mem[][9] );
void cpu_6502_ROR_imp(cpu* cpu, char mem[][9] );
void cpu_6502_JMP_ind(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_abs(cpu* cpu, char mem[][9] );
void cpu_6502_ROR_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BVS_rel(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_izy(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_ROR_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_SEI_imp(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_aby(cpu* cpu, char mem[][9] );
void cpu_6502_ADC_abx(cpu* cpu, char mem[][9] );
void cpu_6502_ROR_abx(cpu* cpu, char mem[][9] );
void cpu_6502_STA_izx(cpu* cpu, char mem[][9] );
void cpu_6502_STY_zp(cpu* cpu, char mem[][9] );
void cpu_6502_STA_zp(cpu* cpu, char mem[][9] );
void cpu_6502_STX_zp(cpu* cpu, char mem[][9] );
void cpu_6502_DEY_imp(cpu* cpu, char mem[][9] );
void cpu_6502_TXA_imp(cpu* cpu, char mem[][9] );
void cpu_6502_STY_abs(cpu* cpu, char mem[][9] );
void cpu_6502_STA_abs(cpu* cpu, char mem[][9] );
void cpu_6502_STX_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BCC_rel(cpu* cpu, char mem[][9] );
void cpu_6502_STA_izy(cpu* cpu, char mem[][9] );
void cpu_6502_STY_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_STA_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_STX_zpy(cpu* cpu, char mem[][9] );
void cpu_6502_TYA_imp(cpu* cpu, char mem[][9] );
void cpu_6502_STA_aby(cpu* cpu, char mem[][9] );
void cpu_6502_TXS_imp(cpu* cpu, char mem[][9] );
void cpu_6502_STA_abx(cpu* cpu, char mem[][9] );
void cpu_6502_LDY_imm(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_izx(cpu* cpu, char mem[][9] );
void cpu_6502_LDX_imm(cpu* cpu, char mem[][9] );
void cpu_6502_LDY_zp(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_zp(cpu* cpu, char mem[][9] );
void cpu_6502_LDX_zp(cpu* cpu, char mem[][9] );
void cpu_6502_TAY_imp(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_imm(cpu* cpu, char mem[][9] );
void cpu_6502_TAX_imp(cpu* cpu, char mem[][9] );
void cpu_6502_LDY_abs(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_abs(cpu* cpu, char mem[][9] );
void cpu_6502_LDX_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BCS_rel(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_izy(cpu* cpu, char mem[][9] );
void cpu_6502_LDY_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_LDX_zpy(cpu* cpu, char mem[][9] );
void cpu_6502_CLV_imp(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_aby(cpu* cpu, char mem[][9] );
void cpu_6502_TSX_imp(cpu* cpu, char mem[][9] );
void cpu_6502_LDY_abx(cpu* cpu, char mem[][9] );
void cpu_6502_LDA_abx(cpu* cpu, char mem[][9] );
void cpu_6502_LDX_aby(cpu* cpu, char mem[][9] );
void cpu_6502_CPY_imm(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_izx(cpu* cpu, char mem[][9] );
void cpu_6502_CPY_zp(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_zp(cpu* cpu, char mem[][9] );
void cpu_6502_DEC_zp(cpu* cpu, char mem[][9] );
void cpu_6502_INY_imp(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_imm(cpu* cpu, char mem[][9] );
void cpu_6502_DEX_imp(cpu* cpu, char mem[][9] );
void cpu_6502_CPY_abs(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_abs(cpu* cpu, char mem[][9] );
void cpu_6502_DEC_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BNE_rel(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_izy(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_DEC_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_CLD_imp(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_aby(cpu* cpu, char mem[][9] );
void cpu_6502_CMP_abx(cpu* cpu, char mem[][9] );
void cpu_6502_DEC_abx(cpu* cpu, char mem[][9] );
void cpu_6502_CPX_imm(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_izx(cpu* cpu, char mem[][9] );
void cpu_6502_CPX_zp(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_zp(cpu* cpu, char mem[][9] );
void cpu_6502_INC_zp(cpu* cpu, char mem[][9] );
void cpu_6502_INX_imp(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_imm(cpu* cpu, char mem[][9] );
void cpu_6502_NOP_imp(cpu* cpu, char mem[][9] );
void cpu_6502_CPX_abs(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_abs(cpu* cpu, char mem[][9] );
void cpu_6502_INC_abs(cpu* cpu, char mem[][9] );
void cpu_6502_BEQ_rel(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_izy(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_INC_zpx(cpu* cpu, char mem[][9] );
void cpu_6502_SED_imp(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_aby(cpu* cpu, char mem[][9] );
void cpu_6502_SBC_abx(cpu* cpu, char mem[][9] );
void cpu_6502_INC_abx(cpu* cpu, char mem[][9] );
