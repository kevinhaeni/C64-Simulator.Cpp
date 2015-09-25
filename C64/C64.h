#ifndef C64_H
#define C64_H
#include <cstdint>
#include "Memory.h"
#include "SID.h"

// Type definitions, constants etc.


//  Define the SIDs address space for memory redirect
#define SID_ADDRESS_SPACE 0x0DFF
#define SID_ADDRESS_SPACE_SIZE 29

class CPU;				// forward declaration

class C64
{
	private:
		Memory *mem;
		CPU *cpu;
		SID *sid;

	public:
		C64();

		// Memory mapping (IMPORTANT: only use these functions for memory access!!)
		void C64::writeMemory(uint16_t address, uint8_t data);
		uint8_t C64::readMemory(uint16_t address);

		void run();
		void reset();
		void test();
	};


#endif