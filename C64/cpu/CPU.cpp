#include "CPU.h"
#include "../util/Utils.h"

#include <string>
#include <iostream>
#include <algorithm>    // std::find_if


/*
	Constructor
*/
CPU::CPU(Memory* memory)
	:memory(memory)
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

/*
	Flags
*/
void CPU::Flags::reset(){
	this->O.clear();
	this->S.clear();
	this->Z.clear();
}

/*
	Print all flags
*/

void CPU::Flags::dump(){
	std::cout << "Flag-Dump:" << std::endl;
	std::cout << "O = " << (int)O.get() << std::endl;
	std::cout << "S = " << (int)S.get() << std::endl;
	std::cout << "Z = " << (int)Z.get() << std::endl;
	std::cout << std::endl;
}


/*
Registers
*/
void CPU::Registers::reset(){
	this->A = 0x00;
	this->X = 0x00;
	this->Y = 0x00;
	this->SP = 0x00;
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

void CPU::doCycle(){
	cycleCounter--;

	// Check if there are cycles left of the previous instruction
	if (cycleCounter <= 0){

		// get OP-Code from program counter
		byte opcode = this->memory->read_byte(this->Registers.PC);

		// decode instruction
		Instruction* inst = this->decodeInstruction(opcode);
		if (inst != nullptr){
			// remember how many cycles the next instruction will require
			cycleCounter = inst->getNumberOfCycles();

			// execute instruction
			inst->execute(this);

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

/*
	Initialize the InstructionTable
*/
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
		(new Instruction(0xA9, "LDA", 3, [](CPU* cpu) {
			// read the byte after opcode and load it into RegA
			byte data = cpu->memory->read_byte(++(cpu->Registers.PC));
			cpu->Registers.A = data;
		}))->getPair());

	// STA
	instructionTable.insert(
		(new Instruction(0x8D, "STA", 4, [](CPU* cpu) {
			
			std::cout << "execute STA" << std::endl;

		}))->getPair());


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