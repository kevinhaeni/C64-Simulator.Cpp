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
public:
	// ctor
	Instruction(int opcode, char *mnemonic, int numOfCycles, std::function<void(CPU*)> func);

	// getter
	uint8_t getOpcode();
	int getNumberOfCycles();
	char* getMnemonicCode();

	// methods
	std::pair<int, Instruction*> getPair();
	std::function<void(CPU*)> execute;

};

#endif