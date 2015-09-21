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
private:
	std::unordered_map<int, Instruction*> instructionTable;
	int cycleCounter;

	byte fetchPCByte();
	byte fetchByteAfterPC();
	word fetchPCWord();
	byte Immediate_Read();
	byte Absolute_Read();
	void Absolute_Write(byte data);
	byte AbsoluteX_Read();
	void AbsoluteX_Write(byte data);
	byte AbsoluteY_Read();
	void AbsoluteY_Write(byte data);
	byte ZeroPage_Read();
	void ZeroPage_Write(byte data);
	byte ZeroPageX_Read();
	void ZeroPageX_Write(byte data);
	byte IndirectX_Read();
	void IndirectX_Write(byte data);
	byte IndirectY_Read();
	void IndirectY_Write(byte data);

public:

	CPU();

	Memory mem;

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

		uint8_t getPCHigh();
		uint8_t getPCLow();		

	} Registers;

	struct Interrupts{
	public:
		bool CIA1;
		bool CIA2;

		bool NMI;
		bool IRQ;

		void reset();
	} Interrupts;


	void resetCPU();
	
	void loadInstructionSet();
	Instruction* decodeInstruction(int opcode);
	
	int emulateCycles(int cyclesToExecute);
	void wasteCycle();

	void triggerCIA1Interrupt();
	void triggerCIA2Interrupt();
	void triggerNMIInterrupt();
	void triggerIRQInterrupt();

};

#endif