#include "C64.h"

C64::C64(){
	// initialize the components
	this->ram = new Memory(65535);
	this->cpu = new CPU(this->ram);
}

void C64::doSomeThings(){
	// How to write to flags
	cpu->Flags.O.set();
	cpu->Flags.dump();

	// How to write to registers
	cpu->Registers.A = 0x05;
	cpu->Registers.dump();

	// how to write to RAM:
	//cpu.memory->write(1, 5);
	//cpu.memory->dump("c:\\temp\\memdump.txt", true);

	// decode and execute an instruction:

	Instruction* i = cpu->decodeInstruction(169);
	if (i != nullptr)
		i->execute(cpu);

	i = cpu->decodeInstruction(150);
	if (i != nullptr)
		i->execute(cpu);

	i = cpu->decodeInstruction(155);
	if (i != nullptr)
		i->execute(cpu);
}