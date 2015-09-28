#include "CPU.h"
#include <string>
#include <iostream>
#include <algorithm>    // std::find_if

CPU::CPU(C64* c64, SID* sid)
	: c64(c64), sid(sid)
{
	this->loadInstructionSet();
	this->resetCPU();
}


void CPU::resetCPU(){
	this->Flags.reset();
	this->Registers.reset();

	this->cycleCounter = 0;
}

/* 
	Flags
*/
void CPU::Flags::reset(){
	N = Z = C = I = D = B = V = false;
}

//	Print all flags
void CPU::Flags::dump(){
	std::cout << "Flag-Dump [";
	std::cout << "C=" << (int)C << ", Z=" << (int)Z << ", I=" << (int)I << ", D=" << (int)D << ", B=" << (int)B << ", V=" << (int)V << ", N=" << (int)N << "]" << std::endl;
}

void CPU::Flags::checkNZ(byte value){
	checkN(value);
	checkZ(value);
}

void CPU::Flags::checkN(byte value){
    N =  value >> 7;
}

void CPU::Flags::checkZ(byte value){
    Z = value == 0 ? true : false;
}

void CPU::Flags::checkC_LSB(byte value){
    C = (value & 0x01) != 0;
}

void CPU::Flags::checkC_MSB(byte value){
    C = (value & 0x80) != 0;
}


byte CPU::Flags::getFlagsAsByte(){
    byte flagsAsByte = 0;
    bool flags[8] = {C,Z,I,D,B,V,false,N};
    
    for (int i=0; i < 8; ++i){
        if(flags[i]){
             flagsAsByte |= 1 << i;
        }
    }
    return flagsAsByte;
}

void CPU::Flags::setFlagsFromByte(byte flags){
    this->N = flags & 0x80;
    this->V = flags & 0x40;
    this->B = flags & 0x10;
    this->D = flags & 0x08;
    this->I = flags & 0x04;
    this->Z = flags & 0x02;
    this->C = flags & 0x01;
}

/*
    Fetch PC
*/
byte CPU::fetchPCByte(){
    return this->c64->readMemory(Registers.PC++);
}word CPU::fetchPCWord(){
	byte lowByte = fetchPCByte();
	byte highByte = fetchPCByte();
    return ((highByte << 8)| lowByte);
}

/*
    Registers
*/
void CPU::Registers::reset(){
	this->A = 0x00;
	this->X = 0x00;
	this->Y = 0x00;
	this->SP = 0xFF; // 0x01FF --> 0x0100
	this->PC = 0x0000;
}

/*
	Print all Registers
*/
void CPU::Registers::dump(){
	std::cout << "Register-Dump:" << std::endl;
	std::cout << "A = " << Utils::hexify(this->A) << std::endl;
	std::cout << "X = " << Utils::hexify(this->X) << std::endl;
	std::cout << "Y = " << Utils::hexify(this->Y) << std::endl;
	std::cout << "SP = " << Utils::hexify(this->SP) << std::endl;
	std::cout << "PC = " << Utils::hexify(this->PC) << std::endl;
	std::cout << std::endl;
}


// Emulate a CPU Cycle
int CPU::emulateCycles(int cyclesToExecute){

	int cycleCounter = cyclesToExecute;

	// get OP-Code from program counter
	byte opcode = fetchPCByte();

	// decode instruction
	Instruction* inst = this->decodeInstruction(opcode);
	if (inst != nullptr){
		// remember how many cycles the next instruction will require
		cycleCounter -= inst->getNumberOfCycles();

#ifdef _DEBUG
		std::cout << "[" << Utils::hexify(Registers.PC) << "] Executing " << inst->getMnemonicCode() << "(" << Utils::hexify(inst->getOpcode()) << ")" << std::endl;
#endif

		// execute instruction
		inst->execute();

		// increase PC
		//this->Registers.PC++;
	}
	else{
#ifdef _DEBUG
		std::cout << "OP-Code " << opcode << " is unknown." << std::endl;
#endif
	}
	
	return (cyclesToExecute - cycleCounter);

}

void CPU::triggerCIA1Interrupt(){
	this->Interrupts.CIA1 = true;

	/*
	 Each of the two CIA chips has five possible sources for interrupts: 
	 underflow of timer A or B, Time of day ALARM (for example, you can set an alarm to occur at 1 p.m. provided that you have set the current time in the CIA chip in the TOD registers), serial port full/empty and FLAG (I'm not sure what FLAG is). The operating system uses one of the timers of one of the two CIA chips to give an IRQ 50 or 60 times per second (depending on if you have a PAL or an NTSC C64). This is necessary for doing such things as checking if a key has been pressed or flashing the cursor. 

	*/
}

void CPU::triggerCIA2Interrupt(){
	this->Interrupts.CIA2 = true;
}

void CPU::triggerNMIInterrupt(){
	this->Interrupts.NMI = true;
}

void CPU::triggerIRQInterrupt(){
	this->Interrupts.IRQ = true;
}

void CPU::Interrupts::reset(){
	CIA1 = false;
	CIA2 = false;
	NMI = false;
	IRQ = false;
}

/*
Lookup of opcode within the instruction table
returns nullptr if the OP-Code does not exist
*/
Instruction* CPU::decodeInstruction(uint8_t opcode){

	auto it = instructionTable.find(opcode);
	if (it != instructionTable.end()) {
		return it->second;
	}
	else
	{
		std::cout << "Invalid OP-Code: " << Utils::hexify(opcode) << std::endl;
		//throw std::exception("Invalid OP-Code");
		//this->resetCPU();
	}			

	return nullptr;
}

void CPU::addInstruction(Instruction* instr){
	// getPair() returns an OPCode + Instruction Object pair ready to be stored in the hashmap
	instructionTable.insert((instr)->getPair());
}


/////////////////////////////////////////////////////////////////////////////////////
// 6502 Addressing modes
/////////////////////////////////////////////////////////////////////////////////////

word CPU::Immediate()
{
	const word immediateAddress = Registers.PC++;
	return immediateAddress;
}

word CPU::Absolute()
{
	const word absoluteAddress = fetchPCWord();
	return absoluteAddress;

}

word CPU::AbsoluteX()
{
	const word absoluteAddress = fetchPCWord() + Registers.X;
	return absoluteAddress;
}

word CPU::AbsoluteY()
{
	const word absoluteAddress = fetchPCWord() + Registers.Y;
	return absoluteAddress;
}

word CPU::ZeroPage()
{
	byte loAddress = fetchPCByte();
	const word zpAddress = Utils::makeWord(loAddress, 0x00);
	return zpAddress;
}

word CPU::ZeroPageX()
{
	byte loAddress = fetchPCByte() + Registers.X;
	const word zpxAddress = Utils::makeWord(loAddress, 0x00);
	return zpxAddress;
}

word CPU::ZeroPageY()
{
	byte loAddress = fetchPCByte() + Registers.Y;
	const word zpyAddress = Utils::makeWord(loAddress, 0x00);
	return zpyAddress;
}

word CPU::Indirect()
{
	byte data = fetchPCByte();
	word zpAddress = Utils::makeWord(data, 0x00);
	const word effAddress = Utils::makeWord(c64->readMemory(zpAddress), c64->readMemory(zpAddress + 1));
	return effAddress;
}


word CPU::IndirectX()
{
	byte data = fetchPCByte() + Registers.X;
	word zpAddress = Utils::makeWord(data, 0x00);
	const word effAddress = Utils::makeWord(c64->readMemory(zpAddress), c64->readMemory(zpAddress + 1));	
	return effAddress;
}

word CPU::IndirectY()
{
	byte data = fetchPCByte();
	word zpAddress = Utils::makeWord(data, 0x00);
	word effAddress = Utils::makeWord(c64->readMemory(zpAddress), c64->readMemory(zpAddress + 1)); // TODO: little / big endian????
	effAddress = effAddress + Registers.Y;
	return effAddress;
}


void CPU::loadRegister(byte* reg, byte value) {
	*reg = value;
    Flags.checkN(value);
    Flags.checkZ(value);
}

void CPU::loadRegister(byte* reg, word addr){
	loadRegister(reg, c64->readMemory(addr));
}

void CPU::push(byte value) {
	const word address = Utils::makeWord(Registers.SP--, 0x01);
	c64->writeMemory(address, value);	
}

byte CPU::pop(){
	const word address = Utils::makeWord(++(Registers.SP), 0x01);
	return c64->readMemory(address);
}

void CPU::inc(word adr) {
	byte value = c64->readMemory(adr);
	value = value++;
	Flags.checkN(value);
	Flags.checkZ(value);
	c64->writeMemory(adr, value);
}

void CPU::dec(word adr) {
	byte value = c64->readMemory(adr);
	value = value--;
	Flags.checkN(value);
	Flags.checkZ(value);
	c64->writeMemory(adr, value);
}

void CPU::AddWithCarry(const byte value) {
	byte tmp = Registers.A + value + (Flags.C ? 1 : 0);
	Flags.C = tmp > 0xFF;
	// The overflow flag is set when the sign of the addends is the same and differs from the sign of the sum
	Flags.V = ~(Registers.A ^ value) & (Registers.A ^ tmp) & 0x80;
	loadRegister(&(Registers.A), tmp);
	
}

void CPU::SubtractWithCarry(const byte value) {
	Flags.C = !Flags.C;					// switch carry flag first
	AddWithCarry(value ^ 0xff);			// Subtraction is the addition of the complement
}


/* ShiftLeft */
void CPU::shiftLeft(byte value){
    Flags.checkC_MSB(value);
    Registers.A = (value << 1) ;
    Flags.checkZ(Registers.A);
    Flags.checkN(Registers.A);
}

void CPU::shiftLeft(word addr){
    byte value = c64->readMemory(addr);
    Flags.checkC_MSB(value);
    value <<= 1;
    Flags.checkZ(value);
    Flags.checkN(value);
    c64->writeMemory(addr,value);
}

/* ShiftRight */
void CPU::shiftRight(byte value){
    Flags.checkC_LSB(value);
    Registers.A = (value >> 1) ;
    Flags.checkZ(Registers.A);
    Flags.N = 0;
}

void CPU::shiftRight(word addr){
    byte value = c64->readMemory(addr);
    Flags.checkC_LSB(value);
    value >>= 1;
    Flags.checkZ(value);
    Flags.N = 0;
    c64->writeMemory(addr,value);
}

/* AND */
void CPU::andRegA(byte operand){
    operand &= Registers.A;
    Flags.checkN(operand);
    Flags.checkZ(operand);
    Registers.A = operand;
}

void CPU::andRegA(word addr){
    byte operand = c64->readMemory(addr);
    andRegA(operand);
}

/* EOR */
void CPU::eorRegA(word addr){
    byte operand = c64->readMemory(addr);
    eorRegA(operand);
}

void CPU::eorRegA(byte operand){
    operand ^= Registers.A;
    Flags.checkN(operand);
    Flags.checkZ(operand);
    Registers.A = operand;
}

/* ORA */
void CPU::oraRegA(word addr){
    byte operand = c64->readMemory(addr);
    oraRegA(operand);
}

void CPU::oraRegA(byte operand){
    operand |= Registers.A;
    Flags.checkN(operand);
    Flags.checkZ(operand);
    Registers.A = operand;
}

/* Rotate Bit Left */
void CPU::rotateBitLeft(byte value){
    // svae Flag C State
    bool  flagCTemp = Flags.C;
    Flags.checkC_MSB(value);
    value = value << 1;
    if(flagCTemp == true) value |= 0x01;
    Flags.checkN(value);
    Flags.checkZ(value);
    Registers.A = value;
}

void CPU::rotateBitLeft(word addr){
    // svae Flag C State
    bool  flagCTemp = Flags.C;
    byte value = c64->readMemory(addr);
    Flags.checkC_MSB(value);
    value = value << 1;
    if(flagCTemp == true) value |= 0x01;
    Flags.checkN(value);
    Flags.checkZ(value);
    c64->writeMemory(addr, value);
}


/* Rotate Bit Right */
void CPU::rotateBitRight(byte value){
    // svae Flag C State
    bool  flagCTemp = Flags.C;
    Flags.checkC_LSB(value);
    value = value >> 1;
    if(flagCTemp == true) value |= 0x80;
    Flags.checkN(value);
    Flags.checkZ(value);
    Registers.A = value;
}

void CPU::rotateBitRight(word addr){
    // svae Flag C State
    bool  flagCTemp = Flags.C;
    byte value = c64->readMemory(addr);
    Flags.checkC_LSB(value);
    value = value >> 1;
    if(flagCTemp == true) value |= 0x80;
    Flags.checkN(value);
    Flags.checkZ(value);
    c64->writeMemory(addr, value);
}

void CPU::compare(byte *reg, word address){
	byte val1 = *reg;
	byte val2 = c64->readMemory(address);
	byte result = val1 - val2;
	Flags.C = (val1 >= val2);
	Flags.checkNZ(result);
}

void CPU::bit(word address){
	byte value = c64->readMemory(address);
	if (value & (1 << 6)) {
		Flags.V = true;
	}
	else {
		Flags.V = false;
	}
	Flags.checkNZ(value);
}

/*
 Initialize the instruction set hashmap
*/
void CPU::loadInstructionSet(){
	/*
	Instruction constructor:
		1. parameter: OP-Code (8bit hex)
		2. parameter: Mnemonic code (3 characters max)
		3. parameter: Number of cycles requires
		4. parameter: Lambda-expression function which performs the actual command. A pointer to the CPU is provided
	*/

    
    /*
     LDY GROUP
     Load Memory data to Register Y
     */
    
    // A0: LDY immediate
    addInstruction(new Instruction(0xA0, "LDY_imm", 2, [this]() {
        loadRegister(&Registers.Y, Immediate());
    }));
    // A4: LDY ZeroPage
    addInstruction(new Instruction(0xA4, "LDY_zp", 3, [this]() {
        loadRegister(&Registers.Y, ZeroPage());
    }));
    // B4: LDY ZeroPageX
    addInstruction(new Instruction(0xB4, "LDY_zpx", 4, [this]() {
        loadRegister(&Registers.Y, ZeroPageX());
    }));
    // AC: LDY Absolute
    addInstruction(new Instruction(0xAC, "LDY_abs", 4, [this]() {
        loadRegister(&Registers.Y, Absolute());
    }));
    // BC: LDY AbsoluteX
    addInstruction(new Instruction(0xBC, "LDY_absx", 4, [this]() {
        loadRegister(&Registers.Y, AbsoluteX());
    }));
    
    
    /*
     LDX GROUP
     Load Memory data to Register X
     */
    
    // A2: LDX immediate
    addInstruction(new Instruction(0xA2, "LDX_imm", 2, [this]() {
        loadRegister(&Registers.X, Immediate());
    }));
    // A6: LDX ZeroPage
    addInstruction(new Instruction(0xA6, "LDX_zp", 3, [this]() {
        loadRegister(&Registers.X, ZeroPage());
    }));
    // B6: LDX ZeroPageX
    addInstruction(new Instruction(0xB6, "LDX_zpx", 4, [this]() {
        loadRegister(&Registers.X, ZeroPageX());
    }));
    // AE: LDX Absolute
    addInstruction(new Instruction(0xAE, "LDX_abs", 4, [this]() {
        loadRegister(&Registers.X, Absolute());
    }));
    // BE: LDX AbsoluteX
    addInstruction(new Instruction(0xBE, "LDX_absx", 4, [this]() {
        loadRegister(&Registers.X, AbsoluteX());
    }));
    
    
    /*
     LDA GROUP
     Load Memory data to Register A
     */

	// A9: LDA ZeroPage
	addInstruction(new Instruction(0xA9, "LDA_imm", 2, [this]() {
		loadRegister(&Registers.A, Immediate());
	}));

    // A5: LDA ZeroPage
    addInstruction(new Instruction(0xA5, "LDA_zp", 3, [this]() {
        loadRegister(&Registers.A, ZeroPage());
    }));
    // B5: LDA ZeroPageX
    addInstruction(new Instruction(0xB5, "LDA_zpx", 4, [this]() {
        loadRegister(&Registers.A, ZeroPageX());
    }));
    // AD: LDA Absolute
    addInstruction(new Instruction(0xAD, "LDA_abs", 4, [this]() {
        loadRegister(&Registers.A, Absolute());
    }));
    // BD: LDA AbsoluteX
    addInstruction(new Instruction(0xBD, "LDA_absx", 4, [this]() {
        loadRegister(&Registers.A, AbsoluteX());
    }));
    // B9: LDA AbsoluteY
    addInstruction(new Instruction(0xB9, "LDA_absy", 4, [this]() {
        loadRegister(&Registers.A, AbsoluteY());
    }));
    // A1: LDA IndirectX
    addInstruction(new Instruction(0xA1, "LDA_ix", 6, [this]() {
        loadRegister(&Registers.A, IndirectX());
    }));
    // B1: LDA IndirectY
    addInstruction(new Instruction(0xB1, "LDA_iy", 5, [this]() {
        loadRegister(&Registers.A, IndirectY());
    }));
    
    
	/*
        STA GROUP
        Store Register A value to Memory
	*/

	// 85: STA ZeroPage
	addInstruction(new Instruction(0x85, "STA_zp", 3, [this]() {
		c64->writeMemory(ZeroPage(), Registers.A);
	}));
	// 95: STA ZeroPageX
	addInstruction(new Instruction(0x85, "STA_zpx", 3, [this]() {
		c64->writeMemory(ZeroPageX(), Registers.A);
	}));
	// 8D: STA Absolute
	addInstruction(new Instruction(0x85, "STA_abs", 3, [this]() {
		c64->writeMemory(Absolute(), Registers.A);
	}));
    
    /*
     STX GROUP
     Store Register X value to Memory
     */
    
    // 86: STX ZeroPage
    addInstruction(new Instruction(0x86, "STX_zp", 2, [this]() {
        c64->writeMemory(ZeroPage(), Registers.X);
    }));
    // 96: STX ZeroPageY
    addInstruction(new Instruction(0x96, "STX_zpx", 2, [this]() {
        c64->writeMemory(ZeroPageY(), Registers.X);
    }));
    // 8E: STX Absolute
    addInstruction(new Instruction(0x8E, "STX_abs", 3, [this]() {
        c64->writeMemory(Absolute(), Registers.X);
    }));
    
    
    /* TXX Group */
    // Tested: OK
    
    // AA: TAX
    addInstruction(new Instruction(0xAA, "TAX", 2, [this]() {
        loadRegister(&Registers.A, Registers.Y);
    }));
	
    // A8: TAY
    addInstruction(new Instruction(0xA8, "TAY", 2, [this]() {
        loadRegister(&Registers.A, Registers.Y);
    }));
    
    // BA: TSX
    addInstruction(new Instruction(0xBA, "TSX", 2, [this]() {
        loadRegister(&Registers.SP, Registers.X);
    }));
    
    // 8A: TXA
    addInstruction(new Instruction(0x8A, "TXA", 2, [this]() {
        loadRegister(&Registers.X, Registers.A);
    }));

    // 9A: TXS
    addInstruction(new Instruction(0x9A, "TXS", 2, [this]() {
        loadRegister(&Registers.X, Registers.SP);
    }));
    
    // 98: TYA
    addInstruction(new Instruction(0x98, "TYA", 2, [this]() {
        loadRegister(&Registers.Y, Registers.A);
    }));  
    
    /**** AND ***/
    // Todo, variable cycles
    // Tested: OK
    
    // 29: AND
    addInstruction(new Instruction(0x29, "AND_i", 2, [this]() {
        andRegA(Immediate());
    }));
    
    // 25: AND
    addInstruction(new Instruction(0x25, "And_zp", 3, [this]() {
        andRegA(ZeroPage());
    }));
    
    // 35: AND
    addInstruction(new Instruction(0x35, "And_zpx", 4, [this]() {
        andRegA(ZeroPageX());
    }));
    
    // 2D: AND
    addInstruction(new Instruction(0x2D, "And_abs", 4, [this]() {
        andRegA(Absolute());
    }));
    
    // 3D: AND
    addInstruction(new Instruction(0x3D, "And_absx", 4, [this]() {
        andRegA(AbsoluteX());
    }));
    
    // 39: AND
    addInstruction(new Instruction(0x39, "And_absy", 4, [this]() {
        andRegA(AbsoluteY());
    }));
    
    // 21: AND
    addInstruction(new Instruction(0x21, "And_inx", 6, [this]() {
        andRegA(IndirectX());
    }));
    
    // 31: AND
    addInstruction(new Instruction(0x31, "And_iny", 5, [this]() {
        andRegA(IndirectY());
    }));
    
    
    /*** ORA ***/
    // Tested: OK
    
    // 09: ORA
    addInstruction(new Instruction(0x09, "ORA_imm", 2, [this]() {
        oraRegA(Immediate());
    }));
    
    // 05: ORA
    addInstruction(new Instruction(0x05, "ORA_zp", 3, [this]() {
        oraRegA(ZeroPage());
    }));
    
    // 15: ORA
    addInstruction(new Instruction(0x15, "ORA_zpx", 4, [this]() {
        oraRegA(ZeroPageX());
    }));
    
    // 0D: ORA
    addInstruction(new Instruction(0x0D, "ORA_abs", 4, [this]() {
        oraRegA(Absolute());
    }));
    
    // 1D: ORA
    addInstruction(new Instruction(0x1D, "ORA_absx", 4, [this]() {
        oraRegA(AbsoluteX());
    }));
    
    // 19: ORA
    addInstruction(new Instruction(0x19, "ORA_absy", 4, [this]() {
        oraRegA(AbsoluteY());
    }));
    
    // 01: ORA
    addInstruction(new Instruction(0x01, "ORA_inx", 6, [this]() {
        oraRegA(IndirectX());
    }));
    
    // 11: ORA
    addInstruction(new Instruction(0x11, "ORA_iny", 5, [this]() {
        oraRegA(IndirectY());
    }));
    
    
    /*** EOR ***/
    // Tested: OK
    
    // 49: EOR
    addInstruction(new Instruction(0x49, "EOR_imm", 2, [this]() {
        eorRegA(Immediate());
    }));
    
    // 45: EOR
    addInstruction(new Instruction(0x45, "EOR_zp", 3, [this]() {
        eorRegA(ZeroPage());
    }));
    
	// 55: EOR
    addInstruction(new Instruction(0x55, "EOR_zpx", 4, [this]() {
        eorRegA(ZeroPageX());
    }));
    
    /*
     4D: Absolute
     */
    addInstruction(new Instruction(0x4D, "EOR_abs", 4, [this]() {
        eorRegA(Absolute());
    }));
    
	// 5D: EOR
    addInstruction(new Instruction(0x5D, "EOR_absx", 4, [this]() {
        eorRegA(AbsoluteX());
    }));
    
	// 59: EOR
    addInstruction(new Instruction(0x59, "EOR_absy", 4, [this]() {
        eorRegA(AbsoluteY());
    }));
    
	// 41: EOR
    addInstruction(new Instruction(0x41, "EOR_inx", 6, [this]() {
        eorRegA(IndirectX());
    }));
    
	// 51: EOR
    addInstruction(new Instruction(0x51, "EOR_iny", 5, [this]() {
        eorRegA(IndirectY());
    }));

    
	/*** LSR GROUP ***/
    // Tested: OK
    
	// 4A: LSR
    addInstruction(new Instruction(0x4A, "LSR_a", 2, [this]() {
        shiftRight(Registers.A);
    }));
    
	// 46: LSR
    addInstruction(new Instruction(0x46, "LSR_zp", 5, [this]() {
        shiftRight(ZeroPage());
    }));
    
	// 56: LSR
    addInstruction(new Instruction(0x56, "LSR_zpx", 6, [this]() {
        shiftRight(ZeroPageX());
    }));
    
	// 4E: LSR
    addInstruction(new Instruction(0x4E, "LSR_abs", 6, [this]() {
        shiftRight(Absolute());
    }));
    
	// 5E: LSR
    addInstruction(new Instruction(0x5E, "LSR_absx", 7, [this]() {
        shiftRight(AbsoluteX());
    }));
    
    
    /*** ASL GROUP ***/
    // Tested: OK
    
	// 0A: ASL
    addInstruction(new Instruction(0x0A, "ASL_a", 2, [this]() {
        shiftLeft(Registers.A);
    }));
    
	// 06: ASL
    addInstruction(new Instruction(0x06, "ASL_zp", 5, [this]() {
        shiftLeft(ZeroPage());
    }));
    
	// 16: ASL
    addInstruction(new Instruction(0x16, "ASL_zpx", 6, [this]() {
        shiftLeft(ZeroPageX());
    }));
    
	// 0E: ASL
    addInstruction(new Instruction(0x0E, "ASL_abs", 6, [this]() {
        shiftLeft(Absolute());
    }));
    
	// 1E: ASL
    addInstruction(new Instruction(0x1E, "ASL_absx", 7, [this]() {
        shiftLeft(AbsoluteX());
    }));

	/* STACK INSTRUCTIONS */
	// Test: OK

	// 48: PHA
	addInstruction(new Instruction(0x48, "PHA", 3, [this]() {
		push(Registers.A);
	}));

	// 68: PLA
	addInstruction(new Instruction(0x68, "PLA", 4, [this]() {
		loadRegister(&Registers.A, pop());
	}));

	// 08: PHP
    // Tested
	addInstruction(new Instruction(0x08, "PHP", 3, [this]() {
        push(Flags.getFlagsAsByte());
	}));

	// 28: PLP
    // Tested
	addInstruction(new Instruction(0x28, "PLP", 4, [this]() {
        Flags.setFlagsFromByte(pop());
	}));

	/* ALU Instructions */
	// UNTESTED!!

	// 69: ADC
	addInstruction(new Instruction(0x69, "ADC_imm", 2, [this]() {
		AddWithCarry(c64->readMemory(Immediate()));
	}));

	// 65: ADC
	addInstruction(new Instruction(0x65, "ADC_zp", 3, [this]() {
		AddWithCarry(c64->readMemory(ZeroPage()));
	}));

	// 75: ADC
	addInstruction(new Instruction(0x75, "ADC_zpx", 4, [this]() {
		AddWithCarry(c64->readMemory(ZeroPageX()));
	}));

	// 6D: ADC
	addInstruction(new Instruction(0x6D, "ADC_abs", 4, [this]() {
		AddWithCarry(c64->readMemory(Absolute()));
	}));

	// 7D: ADC
	addInstruction(new Instruction(0x7D, "ADC_absx", 4, [this]() {
		AddWithCarry(c64->readMemory(AbsoluteX()));
	}));

	// 79: ADC
	addInstruction(new Instruction(0x79, "ADC_absy", 4, [this]() {
		AddWithCarry(c64->readMemory(AbsoluteY()));
	}));

	// 61: ADC
	addInstruction(new Instruction(0x61, "ADC_idx", 6, [this]() {
		AddWithCarry(c64->readMemory(IndirectX()));
	}));

	// 71: ADC
	addInstruction(new Instruction(0x71, "ADC_idy", 5, [this]() {
		AddWithCarry(c64->readMemory(IndirectY()));
	}));


	// E9: SBC
	addInstruction(new Instruction(0xE9, "SBC_imm", 2, [this]() {
		SubtractWithCarry(c64->readMemory(Immediate()));
	}));

	// E5: SBC
	addInstruction(new Instruction(0xE5, "SBC_zp", 3, [this]() {
		SubtractWithCarry(c64->readMemory(ZeroPage()));
	}));

	// F5: SBC
	addInstruction(new Instruction(0xF5, "SBC_zpx", 4, [this]() {
		SubtractWithCarry(c64->readMemory(ZeroPageX()));
	}));

	// ED: SBC
	addInstruction(new Instruction(0xED, "SBC_abs", 4, [this]() {
		SubtractWithCarry(c64->readMemory(Absolute()));
	}));

	// FD: SBC
	addInstruction(new Instruction(0xFD, "SBC_absx", 4, [this]() {
		SubtractWithCarry(c64->readMemory(AbsoluteX()));
	}));

	// F9: SBC
	addInstruction(new Instruction(0xF9, "SBC_absy", 4, [this]() {
		SubtractWithCarry(c64->readMemory(AbsoluteY()));
	}));

	// E1: SBC
	addInstruction(new Instruction(0xE1, "SBC_idx", 6, [this]() {
		SubtractWithCarry(c64->readMemory(IndirectX()));
	}));

	// F1: SBC
	addInstruction(new Instruction(0xF1, "SBC_idy", 5, [this]() {
		SubtractWithCarry(c64->readMemory(IndirectY()));
	}));

	/* BIT */

	// 24: BIT
	addInstruction(new Instruction(0x24, "BIT_zp", 3, [this]() {
		bit(ZeroPage());
	}));

	// 2C: BIT
	addInstruction(new Instruction(0x2C, "BIT_abs", 4, [this]() {
		bit(Absolute());
	}));

	/* Branch Instructions */
	// UNTESTED

	// 10: BPL (Branch on Plus)
	addInstruction(new Instruction(0x10, "BPL", 2, [this]() {
		if (!Flags.N)
			Registers.PC = Absolute();		
		else
			Registers.PC++;
	}));

	// 30: BMI (Branch on Minus)
	addInstruction(new Instruction(0x30, "BMI", 2, [this]() {
		if (Flags.N) 
			Registers.PC = Absolute();
		else
			Registers.PC++;	
	}));

	// 50: BVC
	addInstruction(new Instruction(0x50, "BVC", 2, [this]() {
		if (!Flags.V)
			Registers.PC = Absolute();
		else
			Registers.PC++;
	}));

	// 70: BVS
	addInstruction(new Instruction(0x70, "BVS", 2, [this]() {
		if (Flags.V)
			Registers.PC = Absolute();
		else
			Registers.PC++;
	}));

	// 90: BCC
	addInstruction(new Instruction(0x90, "BCC", 2, [this]() {
		if (!Flags.C)
			Registers.PC = Absolute();
		else
			Registers.PC++;
	}));

	// B0: BCS
	addInstruction(new Instruction(0xB0, "BCS", 2, [this]() {
		if (Flags.C)
			Registers.PC = Absolute();
		else
			Registers.PC++;
	}));

	// D0: BNE
	addInstruction(new Instruction(0xD0, "BNE", 2, [this]() {
		if (!Flags.Z)
			Registers.PC = Absolute();
		else
			Registers.PC++;
	}));

	// F0: BEQ
	addInstruction(new Instruction(0xF0, "BEQ", 2, [this]() {
		if (Flags.C)
			Registers.PC = Absolute();
		else
			Registers.PC++;
	}));

	/* BRK (Break) */

	// 00: BRK (Break)
	addInstruction(new Instruction(0x00, "BRK", 7, [this]() {
		// Not implemented
	}));

	/* Compare Instructions */

	// C9: CMP
	addInstruction(new Instruction(0xC9, "CMP_imm", 2, [this]() {
		compare(&Registers.A, Immediate());
	}));

	// C5: CMP
	addInstruction(new Instruction(0xC5, "CMP_zp", 3, [this]() {
		compare(&Registers.A, ZeroPage());
	}));

	// D5: CMP
	addInstruction(new Instruction(0xD5, "CMP_zpx", 4, [this]() {
		compare(&Registers.A, ZeroPageX());
	}));

	// CD: CMP
	addInstruction(new Instruction(0xCD, "CMP_abs", 4, [this]() {
		compare(&Registers.A, Absolute());
	}));

	// DD: CMP
	addInstruction(new Instruction(0xDD, "CMP_absx", 4, [this]() {
		compare(&Registers.A, AbsoluteX());
	}));

	// D9: CMP
	addInstruction(new Instruction(0xD9, "CMP_absy", 4, [this]() {
		compare(&Registers.A, AbsoluteY());
	}));

	// C1: CMP
	addInstruction(new Instruction(0xC1, "CMP_idx", 6, [this]() {
		compare(&Registers.A, IndirectX());
	}));

	// D1: CMP
	addInstruction(new Instruction(0xD1, "CMP_idy", 5, [this]() {
		compare(&Registers.A, IndirectY());
	}));

	// E0: CPX
	addInstruction(new Instruction(0xE0, "CPX_imm", 2, [this]() {
		compare(&Registers.X, Immediate());
	}));

	// E4: CPX
	addInstruction(new Instruction(0xE4, "CPX_zp", 3, [this]() {
		compare(&Registers.X, ZeroPage());
	}));

	// EC: CPX
	addInstruction(new Instruction(0xEC, "CPX_abs", 4, [this]() {
		compare(&Registers.X, Absolute());
	}));

	// C0: CPY
	addInstruction(new Instruction(0xC0, "CPY_imm", 2, [this]() {
		compare(&Registers.Y, Immediate());
	}));

	// C4: CPY
	addInstruction(new Instruction(0xC4, "CPY_zp", 3, [this]() {
		compare(&Registers.Y, ZeroPage());
	}));

	// CC: CPY
	addInstruction(new Instruction(0xCC, "CPY_abs", 4, [this]() {
		compare(&Registers.Y, Absolute());
	}));

	/* INC */
	// Test: OK

	// E6: INC
	addInstruction(new Instruction(0xE6, "INC_zp", 5, [this]() {
		inc(ZeroPage());
	}));

	// F6: INC
	addInstruction(new Instruction(0xF6, "INC_zpx", 6, [this]() {
		inc(ZeroPageX());
	}));

	// EE: INC
	addInstruction(new Instruction(0xEE, "INC_abs", 6, [this]() {
		inc(Absolute());
	}));

	// FE: INC
	addInstruction(new Instruction(0xFE, "INC_absx", 7, [this]() {
		inc(AbsoluteX());
	}));

	/* DEC */

	// C6: DEC
	addInstruction(new Instruction(0xC6, "DEC_zp", 5, [this]() {
		dec(ZeroPage());
	}));

	// D6: DEC
	addInstruction(new Instruction(0xD6, "DEC_zpx", 6, [this]() {
		dec(ZeroPageX());
	}));

	// CE: DEC
	addInstruction(new Instruction(0xCE, "DEC_abs", 6, [this]() {
		dec(Absolute());
	}));

	// DE: DEC
	addInstruction(new Instruction(0xDE, "DEC_absx", 7, [this]() {
		dec(AbsoluteX());
	}));

	/* Flag Instructions */

	// 18: CLC
	addInstruction(new Instruction(0x18, "CLC", 2, [this]() {
		Flags.C = false;
	}));

	// 38: SEC
	addInstruction(new Instruction(0x38, "SEC", 2, [this]() {
		Flags.C = true;
	}));

	// 58: CLI
	addInstruction(new Instruction(0x58, "CLI", 2, [this]() {
		Flags.I = false;
	}));

	// 78: SEI
	addInstruction(new Instruction(0x78, "SEI", 2, [this]() {
		Flags.I = true;
	}));

	// B8: CLV
	addInstruction(new Instruction(0xB8, "CLV", 2, [this]() {
		Flags.V = false;
	}));

	// D8: CLD
	addInstruction(new Instruction(0xD8, "CLD", 2, [this]() {
		Flags.D = false;
	}));

	// F8: SED
	addInstruction(new Instruction(0xF8, "SED", 2, [this]() {
		Flags.D = true;
	}));

	/* JMP (Jump) Instructions */

	// 4C: JMP
	addInstruction(new Instruction(0x4C, "JMP_abs", 2, [this]() {
		Registers.PC = Absolute();
	}));

	// 6C: JMP
	addInstruction(new Instruction(0x6C, "JMP_id", 2, [this]() {
		Registers.PC = Indirect();
	}));

	// 20: JSR (Jump Subroutine) // UNTESTED
	addInstruction(new Instruction(0x20, "JSR_abs", 2, [this]() {
		word currentPC = Registers.PC;
		push((uint8_t)(currentPC >> 8));
		push((uint8_t)currentPC);
		Registers.PC = fetchPCWord();
	}));

	/* Rotate Instructions */
    // Tested OK
	// 2A: ROL
	addInstruction(new Instruction(0x2A, "ROL_acc", 2, [this]() {
        rotateBitLeft(Registers.A);
	}));

	// 26: ROL
	addInstruction(new Instruction(0x26, "ROL_zp", 5, [this]() {
		rotateBitLeft(ZeroPage());
	}));

	// 36: ROL
	addInstruction(new Instruction(0x36, "ROL_zpx", 6, [this]() {
		rotateBitLeft(ZeroPageX());
	}));

	// 2E: ROL
	addInstruction(new Instruction(0x2E, "ROL_abs", 6, [this]() {
		rotateBitLeft(Absolute());
	}));

	// 3E: ROL
	addInstruction(new Instruction(0x3E, "ROL_absx", 7, [this]() {
		rotateBitLeft(AbsoluteX());
	}));


	// 6A: ROR
	addInstruction(new Instruction(0x6A, "ROR_acc", 2, [this]() {
		rotateBitRight(Registers.A);
	}));

	// 66: ROR
	addInstruction(new Instruction(0x66, "ROR_zp", 5, [this]() {
		rotateBitRight(ZeroPage());
	}));

	// 76: ROR
	addInstruction(new Instruction(0x76, "ROR_zpx", 6, [this]() {
		rotateBitRight(ZeroPageX());
	}));

	// 6E: ROR
	addInstruction(new Instruction(0x6E, "ROR_abs", 6, [this]() {
		rotateBitRight(Absolute());
	}));

	// 7E: ROR
	addInstruction(new Instruction(0x7E, "ROR_absx", 7, [this]() {
        rotateBitRight(AbsoluteX());
	}));

	/* Other Instructions */

	// EA: NOP
	addInstruction(new Instruction(0xEA, "NOP", 2, [this]() {
		// Not implemented
	}));

	// 40: RTI (Return from Interrupt)
	addInstruction(new Instruction(0x40, "RTI", 6, [this]() {
		// Not implemented
	}));

	// 60: RTI (Return from Subroutine) // UNTESTED
	addInstruction(new Instruction(0x60, "RTS", 6, [this]() {
		word newPC;
		newPC = pop();
		newPC |= (word)pop() << 8;
		newPC++;
		Registers.PC = newPC;
	}));

}

