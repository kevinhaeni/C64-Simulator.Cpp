#include "C64.h"
#include "CPU.h"

	C64::C64(){

		this->mem = new Memory(65535);
		this->sid = new SID();
		this->cpu = new CPU(this, sid);
	}


	uint8_t C64::readMemory(uint16_t address) {
		// if address shows to SID's address space --> redirect the read/write to the SID chip
		if (address >= SID_ADDRESS_SPACE && address < SID_ADDRESS_SPACE + SID_ADDRESS_SPACE_SIZE)
			return sid->read_byte(address);
		else
			// otherwise read/write from/to the memory
			return mem->read_byte(address);
	}

	void C64::writeMemory(uint16_t address, uint8_t data){
		// if address shows to SID's address space --> redirect the read/write to the SID chip
		if (address >= SID_ADDRESS_SPACE && address < SID_ADDRESS_SPACE + SID_ADDRESS_SPACE_SIZE)
			sid->write_byte(address, data);
		else
			// otherwise read/write from/to the memory
			mem->write_byte(address, data);
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

		cpu->Registers.A = 0x01; //hack

		// Load test program to memory
     
		this->writeMemory(0x0050, 0x02);
		this->writeMemory(0x1000, 0x69);
        this->writeMemory(0x1001, 0x02);
        this->writeMemory(0x1002, 0xE9);
        this->writeMemory(0x1003, 0x02);
        
		// Stack intsruction tests	
        cpu->emulateCycles(0);	
		cpu->Registers.dump();
		cpu->emulateCycles(0);
		
		cpu->Registers.dump();
		cpu->Flags.dump();
	
	

        // Tests
        
        /*
         LSR Absolute
         this->writeMemory(0x1000, 0x4E);
         this->writeMemory(0x1001, 0x12);
         this->writeMemory(0x1002, 0x40);
         this->writeMemory(0x4012, 0x19);
        */
        
        /*
         LSR Immediate
         this->writeMemory(0x1000, 0x4A);
         this->writeMemory(0x1001, 0x12);
         */
        
        /*
         AND
         this->writeMemory(0x1000, 0x2D);
         this->writeMemory(0x1001, 0x12);
         this->writeMemory(0x1002, 0x40);
         this->writeMemory(0x4012, 0x19);
         */
	}

