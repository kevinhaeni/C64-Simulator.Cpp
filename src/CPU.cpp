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
	std::cout << "Flag-Dump:" << std::endl;
	std::cout << "C = " << (int)C << std::endl;
	std::cout << "Z = " << (int)Z << std::endl;
	std::cout << "I = " << (int)I << std::endl;
	std::cout << "D = " << (int)D << std::endl;
	std::cout << "B = " << (int)B << std::endl;
	std::cout << "V = " << (int)V << std::endl;
	std::cout << "N = " << (int)N << std::endl;
	std::cout << std::endl;
}


/*
    Registers
*/
void CPU::Registers::reset(){
	this->A = 0x00;
	this->X = 0x00;
	this->Y = 0x00;
	this->SP = 0xFF; // 0x01FF till 0x0100
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

		// execute instruction
		inst->execute();

		// increase PC
		this->Registers.PC++;
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
Instruction* CPU::decodeInstruction(int opcode){

	auto it = instructionTable.find(opcode);
	if (it != instructionTable.end()) {
		return it->second;
	}
	else
	{
		std::cout << "Invalid OP-Code" << std::endl;
		//throw std::exception("Invalid OP-Code");
		this->resetCPU();
	}			

	return nullptr;
}

void CPU::addInstruction(Instruction* instr){
	// getPair() returns an OPCode + Instruction Object pair ready to be stored in the hashmap
	instructionTable.insert((instr)->getPair());
}


byte CPU::fetchPCByte(){
	return this->c64->readMemory(Registers.PC);
}
byte CPU::fetchByteAfterPC(){
	return this->c64->readMemory(++(Registers.PC));
}
word CPU::fetchPCWord(){
	return fetchPCByte() & fetchByteAfterPC();
}

/////////////////////////////////////////////////////////////////////////////////////
// 6502 Addressing modes
/////////////////////////////////////////////////////////////////////////////////////

byte CPU::Immediate()
{
	const byte immAddress = fetchByteAfterPC();
	return immAddress;
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
	byte loAddress = fetchByteAfterPC();
	const word zpAddress = Utils::makeWord(loAddress, 0x00);
	return zpAddress;
}

word CPU::ZeroPageX()
{
	byte loAddress = fetchByteAfterPC() + Registers.X;
	const word zpxAddress = Utils::makeWord(loAddress, 0x00);
	return zpxAddress;
}

word CPU::ZeroPageY()
{
	byte loAddress = fetchByteAfterPC() + Registers.Y;
	const word zpyAddress = Utils::makeWord(loAddress, 0x00);
	return zpyAddress;
}

word CPU::IndirectX()
{
	byte data = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(data, 0x00);
	const word effAddress = Utils::makeWord(c64->readMemory(zpAddress), c64->readMemory(zpAddress + 1));	
	return effAddress;
}

word CPU::IndirectY()
{
	byte data = fetchByteAfterPC();
	word zpAddress = Utils::makeWord(data, 0x00);
	word effAddress = Utils::makeWord(c64->readMemory(zpAddress), c64->readMemory(zpAddress + 1)); // TODO: little / big endian????
	effAddress = effAddress + Registers.Y;
	return effAddress;
}

bool CPU::checkIfNegative(byte number){
    return (number >> 7);
}

void CPU::loadRegister(byte* reg, byte value) {
	*reg = value;
	Flags.Z = value == 0 ? true : false;
	Flags.N = checkIfNegative(value);
}

void CPU::loadRegister(byte* reg, word addr){
	loadRegister(reg, c64->readMemory(addr));
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
    
    // A1: LDX immediate
    addInstruction(new Instruction(0xA1, "LDX_imm", 2, [this]() {
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
    
    // A9: LDA immediate
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
    
    /*
     TAX
     */
    addInstruction(new Instruction(0xAA, "TAX_XXX", 2, [this]() {
        loadRegister(&Registers.A, Registers.Y);
    }));
	
    /*
     TAY
     */
    addInstruction(new Instruction(0xA8, "TAY_XXX", 2, [this]() {
        loadRegister(&Registers.A, Registers.Y);
    }));
    
    /*
     TSX
     */
    addInstruction(new Instruction(0xBA, "TSX_XXX", 2, [this]() {
        loadRegister(&Registers.SP, Registers.X);
    }));
    
    /*
     TXA
     */
    addInstruction(new Instruction(0x8A, "TXA_XXX", 2, [this]() {
        loadRegister(&Registers.X, Registers.A);
    }));

    /*
     TXS
     */
    addInstruction(new Instruction(0x9A, "TXS_XXX", 2, [this]() {
        loadRegister(&Registers.X, Registers.SP);
    }));
    
    /*
     TYA
     */
    addInstruction(new Instruction(0x98, "TYA_XXX", 2, [this]() {
        loadRegister(&Registers.Y, Registers.A);
    }));
    
    
    
    
	/*
	LSR GROUP
	*/

	///* LSR */
	//  LSR_a(){
	//	byte data = Registers.A;
	//	Flags.C = (data & 0x01) != 0;
	//	Registers.A = (data >> 1);
	//	//return flags.nz;
	//}
	//
	// LSR_zp();
	// LSR_zpx();
	// LSR_abs();
	// LSR_absx();
	//


}

