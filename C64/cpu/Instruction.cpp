#include "Instruction.h"


/*
Constructor
*/
Instruction::Instruction(int opcode, char *mnemonic, int numOfCycles, std::function<void(CPU*)> func, CPU* cpu)
	: opcode(opcode), cycles(numOfCycles), func(func), theCPU(cpu)

{
	strcpy_s(this->mnemonic, mnemonic);
}

/* 
 Getter
*/
uint8_t Instruction::getOpcode(){
	return this->opcode;
}

int Instruction::getNumberOfCycles(){
	return this->cycles;
}

char* Instruction::getMnemonicCode(){
	return this->mnemonic;
}

/* 
 Methods
*/

std::pair<int, Instruction*> Instruction::getPair(){
	std::pair<int, Instruction*> pair(this->opcode, this);
	return pair;
}


void Instruction::execute(){
	if (this->func != nullptr){
		this->func(this->theCPU);
	}
	else{	
	}
}
