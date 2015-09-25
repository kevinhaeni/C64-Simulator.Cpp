#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <unordered_map>
#include "Flag.h"
#include "Instruction.h"
#include "Utils.h"
#include "SID.h"
#include "c64.h"

// Type definitions, constants etc.
typedef uint8_t byte;
typedef uint16_t word;



class CPU
{
private:

	C64* c64;													// pointer to the C64 object for (shared) memory access
	SID* sid;
	int cycleCounter;
	std::unordered_map<int, Instruction*> instructionTable;
	void addInstruction(Instruction* instr);


	void loadInstructionSet();									// Insert all instructions into the hashmap
	Instruction* decodeInstruction(int opcode);					// Lookup the instruction in the hashmap

	// PC helpers
	byte fetchPCByte();
	byte fetchByteAfterPC();
	word fetchPCWord();

	/* Return the target address of the particular address mode */
	word Immediate();
	word Absolute();	
	word AbsoluteX();
	word AbsoluteY();
	word ZeroPage();
	word ZeroPageX();
	word ZeroPageY();
	word IndirectX();	
	word IndirectY();

	void loadRegister(byte* reg, word addr);					// Load register with memory data

public:
	CPU(C64* c64, SID* sid);

	// Flags
	struct Flags
	{
	public:
		bool C;		// Carry
		bool I;		// Interrupt
		bool B;		// Break
		bool D;		// Decimal
		bool V;		// Overflow
		byte Z;		// Zero
		byte N;		// Null

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


	// Interrupts
	struct Interrupts{
	public:
		bool CIA1;
		bool CIA2;

		bool NMI;
		bool IRQ;

		void reset();
	} Interrupts;
	

	void resetCPU();	
	
	int emulateCycles(int cyclesToExecute);

	void triggerCIA1Interrupt();
	void triggerCIA2Interrupt();
	void triggerNMIInterrupt();
	void triggerIRQInterrupt();

	

};

#endif