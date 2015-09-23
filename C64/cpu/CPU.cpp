#include "CPU.h"
#include <string>
#include <iostream>
#include <algorithm>    // std::find_if

CPU::CPU(Memory* mem, SID* sid)
	: mem(mem), sid(sid)
{
	this->resetCPU();
}


void CPU::resetCPU(){
	this->Flags.reset();
	this->Registers.reset();
	
	this->Registers.SP = 0x00FF;

	this->cycleCounter = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// Flags
/////////////////////////////////////////////////////////////////////////////////////
void CPU::Flags::reset(){
	C = I = D = B = V = false;
	Z = 0x00;
	N = 0x00;
}

//	Print all flags
void CPU::Flags::dump(){
	std::cout << "Flag-Dump:" << std::endl;
	std::cout << "C = " << (int)C << std::endl;
	std::cout << "Z = " << (int)Z << std::endl;
	std::cout << "I = " << (int)I << std::endl;
	std::cout << "D = " << (int)D << std::endl;
	std::cout << "B = " << (int)B << std::endl;
	std::cout << "V = " << (int)V << std::endl;
	std::cout << "N = " << (int)N << std::endl;
	std::cout << std::endl;
}


/////////////////////////////////////////////////////////////////////////////////////
// Registers
/////////////////////////////////////////////////////////////////////////////////////
void CPU::Registers::reset(){
	this->A = 0x00;
	this->X = 0x00;
	this->Y = 0x00;
	this->SP = 0x00;
	this->PC = 0x0000;
}


/*
	Print all Registers
*/
void CPU::Registers::dump(){
	std::cout << "Register-Dump:" << std::endl;
	std::cout << "A = " << Utils::hexify(this->A) << std::endl;
	std::cout << "X = " << Utils::hexify(this->X) << std::endl;
	std::cout << "Y = " << Utils::hexify(this->Y) << std::endl;
	std::cout << "SP = " << Utils::hexify(this->SP) << std::endl;
	std::cout << "PC = " << Utils::hexify(this->PC) << std::endl;
	std::cout << std::endl;
}


byte CPU::fetchPCByte(){
	return readMemory(Registers.PC);
}
byte CPU::fetchByteAfterPC(){
	return readMemory(++(Registers.PC));
}
word CPU::fetchPCWord(){
	return fetchPCByte() & fetchByteAfterPC();
}

uint8_t CPU::readMemory(word address) {
	// if address shows to SID's address space --> redirect the read/write to the SID chip
	if (address >= SID_ADDRESS_SPACE && address < SID_ADDRESS_SPACE + SID_ADDRESS_SPACE_SIZE)
		return sid->read_byte(address);
	else
		// otherwise read/write from/to the memory
		return mem->read_byte(address);
}

void CPU::writeMemory(word address, byte data){
	// if address shows to SID's address space --> redirect the read/write to the SID chip
	if (address >= SID_ADDRESS_SPACE && address < SID_ADDRESS_SPACE + SID_ADDRESS_SPACE_SIZE)
		sid->write_byte(address, data);
	else
		// otherwise read/write from/to the memory
		mem->write_byte(address, data);
}


/////////////////////////////////////////////////////////////////////////////////////
// Immediate addressing
/////////////////////////////////////////////////////////////////////////////////////

// Returns the value at PC+1
byte CPU::FetchImmediate()
{
	const byte data = fetchByteAfterPC();
	cycleCounter += 1;
	return data;
}

// Reads the memory of address found in PC + PC+1
byte CPU::FetchAbsolute()
{
	word absoluteAddress = fetchPCWord(); 
	byte data = mem->read_byte(absoluteAddress);
	return data;

}

void CPU::WriteAbsolute(byte data)
{
	word absoluteAddress = fetchPCWord();
	mem->write_byte(absoluteAddress, data);
}

byte CPU::FetchAbsoluteX()
{
	word absoluteAddress = fetchPCWord() + Registers.X;
	byte data = mem->read_byte(absoluteAddress);
	return data;
}

void CPU::WriteAbsoluteX(byte data)
{
	word absoluteAddress = fetchPCWord() + Registers.X;
	mem->write_byte(absoluteAddress, data);
}


byte CPU::FetchAbsoluteY()
{
	word absoluteAddress = fetchPCWord() + Registers.Y;
	byte data = mem->read_byte(absoluteAddress);
	return data;
}

void CPU::WriteAbsoluteY(byte data)
{
	word absoluteAddress = fetchPCWord() + Registers.Y;
	mem->write_byte(absoluteAddress, data);
}

byte CPU::FetchZeroPage()
{
	byte loAddress = fetchByteAfterPC();
	byte data = mem->read_byte(Utils::makeWord(loAddress, 0x00));
	return data;
}

void CPU::WriteZeroPage(byte data)
{
	byte loAddress = fetchByteAfterPC();
	mem->write_byte(Utils::makeWord(loAddress, 0x00), data);	
}


byte CPU::FetchZeroPageX()
{
	byte loAddress = fetchByteAfterPC() + Registers.X;
	byte data = mem->read_byte(Utils::makeWord(loAddress, 0x00));
	return data;
}

void CPU::WriteZeroPageX(byte data)
{
	byte loAddress = fetchByteAfterPC() + Registers.X;
	mem->write_byte(Utils::makeWord(loAddress, 0x00), data);
}

byte CPU::FetchZeroPageY()
{
	byte loAddress = fetchByteAfterPC() + Registers.Y;
	byte data = mem->read_byte(Utils::makeWord(loAddress, 0x00));
	return data;
}

void CPU::WriteZeroPageY(byte data)
{
	byte loAddress = fetchByteAfterPC() + Registers.Y;
	mem->write_byte(Utils::makeWord(loAddress, 0x00), data);
}


/////////////////////////////////////////////////////////////////////////////////////
// Indirect addressing
/////////////////////////////////////////////////////////////////////////////////////

byte CPU::FetchIndirectX()
{
	byte data = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(data, 0x00);
	word effAddress = Utils::makeWord(mem->read_byte(zpAddress), mem->read_byte(zpAddress + 1));
	data = mem->read_byte(effAddress);
	return data;
}

void CPU::WriteIndirectX(byte data)
{
	byte adr = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(adr, 0x00);
	word effAddress = Utils::makeWord(mem->read_byte(zpAddress), mem->read_byte(zpAddress + 1)); // TODO: little / big endian????
	mem->write_byte(effAddress, data);
}


byte CPU::FetchIndirectY()
{
	byte data = fetchByteAfterPC();
	word zpAddress = Utils::makeWord(data, 0x00);
	word effAddress = Utils::makeWord(mem->read_byte(zpAddress), mem->read_byte(zpAddress + 1)); // TODO: little / big endian????
	data = mem->read_byte(effAddress+Registers.Y);
	return data;
}

void CPU::WriteIndirectY(byte data)
{
	byte adr = fetchByteAfterPC() + Registers.X;
	word zpAddress = Utils::makeWord(adr, 0x00);
	word effAddress = Utils::makeWord(mem->read_byte(zpAddress), mem->read_byte(zpAddress + 1));
	mem->write_byte(effAddress + Registers.Y, data);
}



// Emulate a CPU Cycle
int CPU::emulateCycles(int cyclesToExecute){

	int cycleCounter = cyclesToExecute;

	fetchDecodeExecute();
	this->Registers.PC++;
	
	return (cyclesToExecute - cycleCounter);

}

void CPU::triggerCIA1Interrupt(){
	this->Interrupts.CIA1 = true;

	/*
	 Each of the two CIA chips has five possible sources for interrupts: 
	 underflow of timer A or B, Time of day ALARM (for example, you can set an alarm to occur at 1 p.m. provided that you have set the current time in the CIA chip in the TOD registers), serial port full/empty and FLAG (I'm not sure what FLAG is). The operating system uses one of the timers of one of the two CIA chips to give an IRQ 50 or 60 times per second (depending on if you have a PAL or an NTSC C64). This is necessary for doing such things as checking if a key has been pressed or flashing the cursor. 

	*/
}

void CPU::triggerCIA2Interrupt(){
	this->Interrupts.CIA2 = true;
}

void CPU::triggerNMIInterrupt(){
	this->Interrupts.NMI = true;
}

void CPU::triggerIRQInterrupt(){
	this->Interrupts.IRQ = true;
}

void CPU::Interrupts::reset(){
	CIA1 = false;
	CIA2 = false;
	NMI = false;
	IRQ = false;
}

void CPU::fetchDecodeExecute(){
	// lookup the instruction function in the functionPointerLookupTable
	(*this.*opcodeMap[fetchPCByte()])();
}

/*
 OPCODES
*/
void CPU::NOP()
{

}


void CPU::NI()
{
// Not implemented!!
	std::cout << "Instruction not implemented!" << std::endl;
}

void CPU::LDA_i()
{
	Registers.A = FetchImmediate();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 2;
}

void CPU::LDA_zp(){
	Registers.A = FetchZeroPage();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	
	cycleCounter += 3;
}

void CPU::LDA_zpx(){
	Registers.A = FetchZeroPageX();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4;
}

void CPU::LDA_abs(){
	Registers.A = FetchAbsolute();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4;
}

void CPU::LDA_absx(){
	Registers.A = FetchAbsoluteX();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4; //+1 if page boundary crossed
}

void CPU::LDA_absy(){
	Registers.A = FetchAbsoluteY();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4; //+1 if page boundary crossed
}

void CPU::LDA_idx(){
	Registers.A = FetchIndirectX();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 6;
}

void CPU::LDA_idy(){
	Registers.A = FetchIndirectY();
	if (Registers.A == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 5; // + 1 if page boundary crossed
}

/* LDX */
void CPU::LDX_i(){
	Registers.X = FetchImmediate();
	if (Registers.X == 0)
		Flags.Z = true;
	else
		Flags.Z = false;
	cycleCounter += 2;
}

void CPU::LDX_zp(){
	Registers.X = FetchZeroPage();
	if (Registers.X == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 3;
}
void CPU::LDX_zpy(){
	Registers.X = FetchZeroPageY();
	if (Registers.X == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4;
}
void CPU::LDX_abs(){
	Registers.X = FetchAbsolute();
	if (Registers.X == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4;
}
void CPU::LDX_absy(){
	Registers.A = FetchAbsoluteY();
	if (Registers.X == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4; // +1 if page boundary crossed
}

/* LDY */
void CPU::LDY_i(){
	Registers.Y = FetchImmediate();
	if (Registers.Y == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 2;
}

void CPU::LDY_zp(){
	Registers.X = FetchZeroPage();
	if (Registers.Y == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 3;
}
void CPU::LDY_zpx(){
	Registers.Y = FetchZeroPageX();
	if (Registers.Y == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4;
}
void CPU::LDY_abs(){
	Registers.Y = FetchAbsolute();
	if (Registers.Y == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4;
}
void CPU::LDY_absx(){
	Registers.Y = FetchAbsoluteX();
	if (Registers.Y == 0)
		Flags.Z = true;
	else
		Flags.Z = false;

	cycleCounter += 4; // +1 if page boundary crossed
}

/* LSR */
void CPU::LSR_a(){
	byte data = Registers.A;
	Flags.C = (data & 0x01) != 0;
	Registers.A = (data >> 1);
	//return flags.nz;
}

void LSR_zp();
void LSR_zpx();
void LSR_abs();
void LSR_absx();

void CPU::STA_zp(){
	byte loAddress = fetchByteAfterPC();
	mem->write_byte(Utils::makeWord(loAddress, 0x00), Registers.A);	
	cycleCounter += 3;
}

void (CPU::*const CPU::opcodeMap[0x100])() =
{
//    0        1         2         3         4          5        6         7         8         9         A         B         C         D         E         F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x00 - 0x0F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x10 - 0x1F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x20 - 0x2F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x30 - 0x3F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x40 - 0x4F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x50 - 0x5F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x60 - 0x6F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x70 - 0x7F
	&NI,      &NI,      &NI,      &NI,      &NI,      &STA_zp,  &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x80 - 0x8F
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0x90 - 0x9F
	&LDY_i,   &LDA_idx, &LDX_i,   &NI,      &LDY_zp,  &LDA_zp,  &LDX_zp,  &NI,      &NI,      &LDA_i,   &NI,      &NI,      &LDY_abs, &LDA_abs, &LDX_abs, &NI,      // 0xA0 - 0xAF
	&NI,      &LDA_idy, &NI,      &NI,      &LDY_zpx, &LDA_zpx, &LDX_zpy, &NI,      &NI,      &NI,      &NI,      &NI,      &LDY_absx,&LDA_absy,&LDX_absy,&NI,      // 0xB0 - 0xBF
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0xC0 - 0xCF
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0xD0 - 0xDF
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0xE0 - 0xEF
	&NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      &NI,      // 0xF0 - 0xFF

};
