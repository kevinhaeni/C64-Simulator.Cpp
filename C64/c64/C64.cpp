#include "C64.h"

	C64::C64(){

		this->mem = new Memory(65535);
		this->sid = new SID();
		this->cpu = new CPU(mem, sid);
	}

	void C64::run(){
		int cycles = 10;
		int previousCycles = 0;

		do{
			// VIC.doCycles();
			// SID.doCycles();
			// Interrupts...
			previousCycles = cpu->emulateCycles(cycles + previousCycles);

		} while (true);

	}

	void C64::reset(){

	}

	//void C64::test(){
	//	// How to write to flags
	//	cpu->Flags.O.set();
	//	cpu->Flags.dump();

	//	// How to write to registers
	//	//cpu->Registers.A = 0x05;
	//	//cpu->Registers.dump();

	//	// how to write to RAM:
	//	//cpu->mem->write(1, 5);
	//	//cpu->mem->dump("c:\\temp\\memdump.txt", true);

	//	// decode and execute an instruction:	
	//	//Instruction* i = cpu->decodeInstruction(0x85);
	//	//if (i != nullptr)
	//	//	i->execute();


	//	// LDA zeropage Write and Read test

	//	cpu->Registers.A = 7;
	//	cpu->Registers.PC = 0x1000;
	//	cpu->mem->write_byte(0x1000, 0x85); // STA zp
	//	cpu->mem->write_byte(0x1001, 0x50);
	//	cpu->mem->write_byte(0x1002, 0xA5); // LDA zp
	//	cpu->mem->write_byte(0x1003, 0x50);

	//	cpu->emulateCycles(3);
	//	cpu->Registers.A = 0x00;
	//	cpu->emulateCycles(3);
	//	cpu->Registers.dump();
	//	cpu->mem->save("c:\\test\\mem->dump");
	//}
	void C64::test(){
		// How to write to flags
		cpu->Flags.O.set();
		cpu->Flags.dump();

		// How to write to registers
		//cpu->Registers.A = 0x05;
		//cpu->Registers.dump();

		// how to write to RAM:
		//cpu->mem->write(1, 5);
		//cpu->mem->dump("c:\\temp\\memdump.txt", true);

		// decode and execute an instruction:	
		//Instruction* i = cpu->decodeInstruction(0x85);
		//if (i != nullptr)
		//	i->execute();


		// LDA zeropage Write and Read test

		cpu->Registers.A = 7;
		cpu->Registers.PC = 0x1000;
		cpu->mem->write_byte(0x1000, 0x85); // STA zp
		cpu->mem->write_byte(0x1001, 0x50);
		cpu->mem->write_byte(0x1002, 0xA5); // LDA zp
		cpu->mem->write_byte(0x1003, 0x50);

		cpu->emulateCycles(3);
		cpu->Registers.A = 0x00;
		cpu->emulateCycles(3);
		cpu->Registers.dump();
		cpu->mem->save("c:\\test\\mem->dump");
	}

