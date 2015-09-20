#include "CPU.h"
#include "../util/Utils.h"

#include <string>
#include <iostream>
#include <algorithm>    // std::find_if


CPU::CPU()
	: mem(65535)
{
	this->loadInstructionSet();
	this->resetCPU();
}


void CPU::resetCPU(){
	this->Flags.reset();
	this->Registers.reset();
	
	this->Registers.SP = 0x00FF;

	this->cycleCounter = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// Flags
/////////////////////////////////////////////////////////////////////////////////////
void CPU::Flags::reset(){
	this->O.clear();
	this->S.clear();
	this->Z.clear();
}

//	Print all flags
void CPU::Flags::dump(){
	std::cout << "Flag-Dump:" << std::endl;
	std::cout << "O = " << (int)O.get() << std::endl;
	std::cout << "S = " << (int)S.get() << std::endl;
	std::cout << "Z = " << (int)Z.get() << std::endl;
	std::cout << std::endl;
}


/////////////////////////////////////////////////////////////////////////////////////
// Registers
/////////////////////////////////////////////////////////////////////////////////////
void CPU::Registers::reset(){
	this->A = 0x00;
	this->X = 0x00;
	this->Y = 0x00;
	this->SP = 0x00;
	this->PC = 0x0000;
}

// Get High-Byte of PC
uint8_t CPU::Registers::getPCHigh(){
	uint8_t hi = (uint8_t)PC; // Get higher byte of 16-bit register
	return hi;
}

// Get Low-Byte of PC
uint8_t CPU::Registers::getPCLow(){
	uint8_t lo = (uint8_t)PC; // Get lower byte of 16-bit register
	return lo;
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


byte CPU::fetchPCByte(){
	return mem.read_byte(Registers.PC);
}
byte CPU::fetchByteAfterPC(){
	return mem.read_byte(++(Registers.PC));
}
word CPU::fetchPCWord(){
	return fetchPCByte() & fetchByteAfterPC();
}

/////////////////////////////////////////////////////////////////////////////////////
// Immediate addressing
/////////////////////////////////////////////////////////////////////////////////////

// Returns the value at PC+1
byte CPU::Immediate_Read()
{
	const byte data = fetchByteAfterPC();
	return data;
}

/////////////////////////////////////////////////////////////////////////////////////
// Absolute addressing
/////////////////////////////////////////////////////////////////////////////////////

// Reads the memory of address found in PC + PC+1
byte CPU::Absolute_Read()
{
	word absoluteAddress = fetchPCWord(); 
	byte data = mem.read_byte(absoluteAddress);
	return data;
}

void CPU::Absolute_Write(byte data)
{
	word absoluteAddress = fetchPCWord();
	mem.write_byte(absoluteAddress, data);
}

byte CPU::AbsoluteX_Read()
{
	word absoluteAddress = fetchPCWord() + Registers.X;
	byte data = mem.read_byte(absoluteAddress);
	return data;
}

void CPU::AbsoluteX_Write(byte data)
{
	word absoluteAddress = fetchPCWord() + Registers.X;
	mem.write_byte(absoluteAddress, data);
}


byte CPU::AbsoluteY_Read()
{
	word absoluteAddress = fetchPCWord() + Registers.Y;
	byte data = mem.read_byte(absoluteAddress);
	return data;
}

void CPU::AbsoluteY_Write(byte data)
{
	word absoluteAddress = fetchPCWord() + Registers.Y;
	mem.write_byte(absoluteAddress, data);
}
/////////////////////////////////////////////////////////////////////////////////////
// Zeropage addressing
/////////////////////////////////////////////////////////////////////////////////////

byte CPU::ZeroPage_Read()
{
	byte loAddress = fetchByteAfterPC();
	byte data = mem.read_byte(Utils::makeWord(loAddress, 0x00));
	return data;
}

void CPU::ZeroPage_Write(byte data)
{
	byte loAddress = fetchByteAfterPC();
	mem.write_byte(Utils::makeWord(loAddress, 0x00), data);	
}


byte CPU::ZeroPageX_Read()
{
	byte loAddress = fetchByteAfterPC() + Registers.X;
	byte data = mem.read_byte(Utils::makeWord(loAddress, 0x00));
	return data;
}

void CPU::ZeroPageX_Write(byte data)
{
	byte loAddress = fetchByteAfterPC() + Registers.X;
	mem.write_byte(Utils::makeWord(loAddress, 0x00), data);
}


/////////////////////////////////////////////////////////////////////////////////////
// Indirect addressing
/////////////////////////////////////////////////////////////////////////////////////

byte CPU::IndirectX_Read()
{
	byte data = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(data, 0x00);
	word effAddress = Utils::makeWord(mem.read_byte(zpAddress), mem.read_byte(zpAddress + 1));
	data = mem.read_byte(effAddress);
	return data;
}

void CPU::IndirectX_Write(byte data)
{
	byte adr = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(adr, 0x00);
	word effAddress = Utils::makeWord(mem.read_byte(zpAddress), mem.read_byte(zpAddress + 1)); // TODO: little / big endian????
	mem.write_byte(effAddress, data);
}


byte CPU::IndirectY_Read()
{
	byte data = fetchByteAfterPC();
	word zpAddress = Utils::makeWord(data, 0x00);
	word effAddress = Utils::makeWord(mem.read_byte(zpAddress), mem.read_byte(zpAddress + 1)); // TODO: little / big endian????
	data = mem.read_byte(effAddress+Registers.Y);
	return data;
}

void CPU::IndirectY_Write(byte data)
{
	byte adr = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(adr, 0x00);
	word effAddress = Utils::makeWord(mem.read_byte(zpAddress), mem.read_byte(zpAddress + 1));
	mem.write_byte(effAddress + Registers.Y, data);
}

/////////////////////////////////////////////////////////////////////////////////////
//	Initialize the InstructionTable
/////////////////////////////////////////////////////////////////////////////////////
void CPU::loadInstructionSet(){
	/*
		Instruction construction:
		1. parameter: OP-Code (8bit hex)
		2. parameter: Mnemonic code (3 characters max)
		3. parameter: Number of cycles requires
		4. parameter: Lambda-expression function which performs the actual command. A pointer to the CPU is provided
		getPair() returns a OP-Code + Instruction Object pair ready to be stored in the hashmap
	*/

	// LDA immediate
	instructionTable.insert(
		(new Instruction(0xA9, "LDA", 2, [](CPU* cpu) {
			// read the byte after opcode and load it into RegA
			cpu->Registers.A = cpu->Immediate_Read();
		}, this))->getPair());

	// LDA Zeropage
	instructionTable.insert(
		(new Instruction(0xA5, "LDA", 3, [](CPU* cpu) {
		// read the byte after opcode and load it into RegA
		cpu->Registers.A = cpu->ZeroPage_Read();
	}, this))->getPair());

	// STA Zeropage
	instructionTable.insert(
		(new Instruction(0x85, "STA", 3, [](CPU* cpu) {
		cpu->ZeroPage_Write(cpu->Registers.A);

		}, this))->getPair());


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
		std::cout << "Invalid OP-Code" << std::endl;
	//throw std::exception("Invalid OP-Code");

	return nullptr;
}

// Emulate a CPU Cycle
void CPU::doCycle(){
	cycleCounter--;

	// Check if there are cycles left of the previous instruction
	if (cycleCounter <= 0){

		// get OP-Code from program counter
		byte opcode = fetchPCByte();

		// decode instruction
		Instruction* inst = this->decodeInstruction(opcode);
		if (inst != nullptr){
			// remember how many cycles the next instruction will require
			cycleCounter = inst->getNumberOfCycles();

			// execute instruction
			inst->execute();

			// increase PC
			this->Registers.PC++;
		}

	}
	else
		wasteCycle();
}

void CPU::wasteCycle(){
	// NOP
}