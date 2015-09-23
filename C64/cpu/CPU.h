#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <unordered_map>
#include "Flag.h"
#include "Memory.h"
#include "../util/Utils.h"
#include "../SID/SID.h"

/*
	Type definitions
*/
typedef uint8_t byte;
typedef uint16_t word;

#define SID_ADDRESS_SPACE 0x0DFF
#define SID_ADDRESS_SPACE_SIZE 29

/*
	CPU Class
*/
class CPU
{
private:
	Memory* mem;
	int cycleCounter;
	
	/* Memory Addressing and ALU functions */
	byte fetchPCByte();
	byte fetchByteAfterPC();
	word fetchPCWord();

	byte FetchImmediate();
	
	byte FetchAbsolute();	
	void WriteAbsolute(byte data);

	byte FetchAbsoluteX();
	void WriteAbsoluteX(byte data);	

	byte FetchAbsoluteY();
	void WriteAbsoluteY(byte data);

	byte FetchZeroPage();
	void WriteZeroPage(byte data);

	byte FetchZeroPageX();
	void WriteZeroPageX(byte data);
	byte FetchZeroPageY();
	void WriteZeroPageY(byte data);

	byte FetchIndirectX();	
	void WriteIndirectX(byte data);

	byte FetchIndirectY();
	void WriteIndirectY(byte data);

public:
	SID* sid;

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

	byte readMemory(word address);
	void writeMemory(word address, byte data);

	void fetchDecodeExecute();			// Fetch PC, decode function pointer and execute function

	//////////////////////////////////////////////////
	// INSTRUCTIONS
	//////////////////////////////////////////////////
	
	void NI();							// Not Implemented placeholder
	void NOP();	

	/* LDA */
	void LDA_i();
	void LDA_zp();
	void LDA_zpx();
	void LDA_zpy();
	void LDA_abs();
	void LDA_absx();
	void LDA_absy();
	void LDA_idx();
	void LDA_idy();
	/* LDX */
	void LDX_i();
	void LDX_zp();
	void LDX_zpy();
	void LDX_abs();
	void LDX_absy();
	/* LDY */
	void LDY_i();
	void LDY_zp();
	void LDY_zpx();
	void LDY_abs();
	void LDY_absx();
	/* LSR */
	void LSR_a();
	void LSR_zp();
	void LSR_zpx();
	void LSR_abs();
	void LSR_absx();

	/* STA */
	void STA_zp();

	static void (CPU::*const opcodeMap[0x100])();
	

};

#endif