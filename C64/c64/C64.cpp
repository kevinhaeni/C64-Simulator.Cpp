#include "C64.h"

	C64::C64(){

		this->mem = new Memory(65535);
		this->sid = new SID();
		this->cpu = new CPU(mem, sid);
	}

	void C64::run(){
		// CLOCK_PAL  =  985248 Hz
		// Considering an average of approx. 3 cycles per instruction, we need 1/3 * 985248 = 328416 iterations per second
		// in order to reach the PAL clock frequency.
		
		int previousCycles = 0;
		do{
			// Check for interrupts...
			// VIC.emulateCycles();
			sid->emulateCycles();			
			previousCycles = cpu->emulateCycles(10 + previousCycles);

		} while (true);

	}

	void C64::reset(){

	}

	
	void C64::test(){
		// LDA zeropage Write and Read test
		
		cpu->Registers.PC = 0x1000;

		cpu->Registers.A = 7; //hack

		// Load test program to memory
		cpu->writeMemory(0x1000, 0x85); // STA zp
		cpu->writeMemory(0x1001, 0x50);
		cpu->writeMemory(0x1002, 0xA5); // LDA zp
		cpu->writeMemory(0x1003, 0x50);

		cpu->emulateCycles(0);
		cpu->Registers.A = 0x00; // hack
		cpu->emulateCycles(0);

		cpu->Registers.dump();
		cpu->Flags.dump();

		cpu->LSR_a();

		cpu->Registers.dump();
		cpu->Flags.dump();
//		cpu->mem->save("c:\\test\\mem->dump");
	}

