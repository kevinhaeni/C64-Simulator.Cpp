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

	this->clockCycleCounter = 0;
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
	clockCycleCounter--;

	if (clockCycleCounter <= 0){

		// get OPCode from program counter
		word opcode = this->memory->read_byte(this->Registers.PC);

		// decode instruction
		Instruction* inst = this->decodeInstruction(opcode);
		if (inst != nullptr){
			// check how many cycles the next instruction will take
			clockCycleCounter = inst->cycles;

			// execute instruction
			inst->execute(this);

			// increase PC
			this->Registers.PC++;
		}

	}
}

/*
	Initialize the InstructionTable
*/
void CPU::loadInstructionSet(){
	// LDA
	instructionTable.insert((new Instruction(169,
		[](CPU* cpu) {
		std::cout << "execute LDA" << std::endl;
	}
	))->getPair());

	// STA
	instructionTable.insert((new Instruction(150,
		[](CPU* cpu) {
		std::cout << "execute STA" << std::endl;
	}
	))->getPair());


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