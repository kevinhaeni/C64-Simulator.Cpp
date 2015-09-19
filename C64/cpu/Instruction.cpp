#include "Instruction.h"


/*
Constructor
*/
Instruction::Instruction(int opcode, char *mnemonic, int numOfCycles, std::function<void(CPU*)> func)
	: opcode(opcode), cycles(numOfCycles), execute(func)

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


