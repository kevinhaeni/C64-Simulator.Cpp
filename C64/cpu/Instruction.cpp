#include "Instruction.h"


/*
Constructor
*/
Instruction::Instruction(int opcode, std::function<void(CPU*)> func){
	this->opcode = opcode;
	this->execute = func;
}

std::pair<int, Instruction*> Instruction::getPair(){
	std::pair<int, Instruction*> pair(this->opcode, this);
	return pair;
}


