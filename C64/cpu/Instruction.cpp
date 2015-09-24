#include "Instruction.h"


/*
Constructor
*/
Instruction::Instruction(int opcode, std::string mnemonic, int numOfCycles, std::function<void()> func)
	: opcode(opcode), mnemonic(mnemonic), cycles(numOfCycles), func(func)

{	
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

std::string Instruction::getMnemonicCode(){
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
		this->func();
	}
	else{
	}
}