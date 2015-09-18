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

public:
	std::string mnemonic;
	uint8_t opcode;
	int cycles;

	Instruction(int opcode, std::function<void(CPU*)> func);

	std::pair<int, Instruction*> getPair();
	std::function<void(CPU*)> execute;

};

#endif