#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <cstdint>
#include <string>
#include <functional>
#include <iostream>

class CPU;
class Instruction
{
private:	
	char mnemonic[4];
	uint8_t opcode;
	int cycles;
	CPU* theCPU;

	// the real implementation of the instruction, will be assigned as a lambda expression later
	std::function<void(CPU*)> func;
public:
	// ctor
	Instruction(int opcode, char *mnemonic, int numOfCycles, std::function<void(CPU*)> func, CPU* cpu);

	// getter
	uint8_t getOpcode();
	int getNumberOfCycles();
	char* getMnemonicCode();

	// methods
	void execute();
	std::pair<int, Instruction*> getPair();	

};

#endif