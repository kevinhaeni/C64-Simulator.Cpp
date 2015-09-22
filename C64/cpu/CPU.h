#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <unordered_map>
#include "Instruction.h"
#include "Flag.h"
#include "Memory.h"
#include "../util/Utils.h"
#include "../SID/SID.h"

/*
	Type definitions
*/
typedef uint8_t byte;
typedef uint16_t word;

#define SID_ADDRESS 0x0DFF

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

	byte read(word address);

public:
	Memory* mem;
	SID* sid;

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

	struct Interrupts{
	public:
		bool CIA1;
		bool CIA2;

		bool NMI;
		bool IRQ;

		void reset();
	} Interrupts;

	CPU(Memory* mem, SID* sid);
	void resetCPU();	
	
	int emulateCycles(int cyclesToExecute);

	void triggerCIA1Interrupt();
	void triggerCIA2Interrupt();
	void triggerNMIInterrupt();
	void triggerIRQInterrupt();

	void fetchDecodeExecute();

	void NI();

	void NOP();
	void LDA_i();
	void LDA_zp();
	void STA_zp();

	static void (CPU::*const opcodeMap[0x100])();
	

};

#endif