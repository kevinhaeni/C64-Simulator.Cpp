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
	uint8_t opcode;
	std::string mnemonic;
	int cycles;

	// the real implementation of the instruction, will be assigned as a lambda expression later
	std::function<void()> func;
public:
	// ctor
	Instruction(int opcode, std::string mnemonic, int numOfCycles, std::function<void()> func);

	// getter
	uint8_t getOpcode();
	int getNumberOfCycles();
	std::string getMnemonicCode();

	// methods
	void execute();
	std::pair<int, Instruction*> getPair();

};

#endif