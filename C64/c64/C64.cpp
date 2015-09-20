#include "C64.h"

C64::C64(){
	// initialize the components
	this->cpu = new CPU();
	this->sid = new SID(this->cpu);
}

void C64::doSomeThings(){
	// How to write to flags
	cpu->Flags.O.set();
	cpu->Flags.dump();

	// How to write to registers
	//cpu->Registers.A = 0x05;
	//cpu->Registers.dump();

	// how to write to RAM:
	//cpu.mem.write(1, 5);
	//cpu.mem.dump("c:\\temp\\memdump.txt", true);

	// decode and execute an instruction:	
	//Instruction* i = cpu->decodeInstruction(0x85);
	//if (i != nullptr)
	//	i->execute();


	// LDA zeropage Write and Read test

	cpu->Registers.A = 7;
	cpu->Registers.PC = 0x1000;
	cpu->mem.write_byte(0x1000, 0x85); // STA zp
	cpu->mem.write_byte(0x1001, 0x50);	
	cpu->mem.write_byte(0x1002, 0xA5); // LDA zp
	cpu->mem.write_byte(0x1003, 0x50);
	
	cpu->doCycle();
	cpu->Registers.A = 0x00;
	cpu->doCycle();
	cpu->Registers.dump();
	cpu->mem.save("c:\\test\\mem.dump");
}