#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <unordered_map>
#include "Flag.h"
#include "Instruction.h"
#include "Utils.h"
#include "SID.h"
#include "C64.h"

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
	byte Immediate();
	word Absolute();	
	word AbsoluteX();
	word AbsoluteY();
	word ZeroPage();
	word ZeroPageX();
	word ZeroPageY();
	word IndirectX();	
	word IndirectY();

	void loadRegister(byte* reg, byte value);
	void loadRegister(byte* reg, word addr);					// Load register with memory data
    
    void shiftLeft(word reg);
    void shiftLeft(byte addr);
    void shiftRight(word reg);
    void shiftRight(byte addr);
  
    void andRegA(word addr);
    void andRegA(byte value);
    
    void eorRegA(word addr);
    void eorRegA(byte value);
    
    void oraRegA(word addr);
    void oraRegA(byte value);
    

public:
	CPU(C64* c64, SID* sid);

	// Flags
	struct Flags
	{
	public:
        void checkC_LSB(byte value);
        void checkC_MSB(byte value);
    
        void checkB(byte value);
        void checkD(byte value);
        void checkV(byte value);
        void checkZ(byte value);
        void checkN(byte value);
        
        bool C;		// Carry
		bool I;		// Interrupt
		bool B;		// Break
		bool D;		// Decimal
		bool V;		// Overflow
		bool Z;		// Zero
		bool N;		// Null

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