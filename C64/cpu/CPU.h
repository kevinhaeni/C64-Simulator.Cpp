#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <unordered_map>
#include "Instruction.h"
#include "Flag.h"
#include "Memory.h"

/*
	Type definitions
*/
typedef uint8_t byte;
typedef uint16_t word;


/*
	CPU Class
*/
class CPU
{
  public:
	Memory* memory;
	std::unordered_map<int, Instruction*> instructionTable;
	uint8_t cycleCounter;

	// ctor
	CPU(Memory* memory);
	void resetCPU();
	void loadInstructionSet();
	Instruction* decodeInstruction(int opcode);
	
	void doCycle();
	void wasteCycle();

	// Flags
	struct Flags 
	{
	public:
		Flag O;		// Overflow
		Flag S;		// Sign
		Flag Z;		// Zero

		void reset();
		void dump();
	} Flags;

	// Registers
	struct Registers{
	public:
		uint8_t A;		// Accumulator
		uint8_t X;		// index X
		uint8_t Y;		// index Y
		uint8_t SP;		// stack pointer
		uint16_t PC;	// program counter
		
		void reset();
		void dump();

	} Registers;
	
};

#endif