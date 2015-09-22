#include "CPU.h"
#include <string>
#include <iostream>
#include <algorithm>    // std::find_if

#define INTERRUPT_INTERVAL 8
#define SID_NUMREGS 29

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
	this->O.clear();
	this->S.clear();
	this->Z.clear();
}

//	Print all flags
void CPU::Flags::dump(){
	std::cout << "Flag-Dump:" << std::endl;
	std::cout << "O = " << (int)O.get() << std::endl;
	std::cout << "S = " << (int)S.get() << std::endl;
	std::cout << "Z = " << (int)Z.get() << std::endl;
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
	return mem->read_byte(Registers.PC);
}
byte CPU::fetchByteAfterPC(){
	return mem->read_byte(++(Registers.PC));
}
word CPU::fetchPCWord(){
	return fetchPCByte() & fetchByteAfterPC();
}

uint8_t CPU::read(word adr) {
	if (adr >= SID_ADDRESS && adr < SID_ADDRESS + SID_NUMREGS)
		return sid->read_byte(adr);
	else
		return mem->read_byte(adr);
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

/////////////////////////////////////////////////////////////////////////////////////
// Absolute addressing
/////////////////////////////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////////////////////////////////////////
// Zeropage addressing
/////////////////////////////////////////////////////////////////////////////////////

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
	// NOP
}

void CPU::LDA_i()
{
	Registers.A = FetchImmediate();
}

void CPU::LDA_zp(){
	Registers.A = FetchZeroPage();
}

void CPU::STA_zp(){
	WriteZeroPage(Registers.A);
}

void (CPU::*const CPU::opcodeMap[0x100])() =
{
//    0        1        2        3        4        5        6        7        8        9        A        B        C        D        E        F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x00 - 0x0F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x10 - 0x1F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x20 - 0x2F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x30 - 0x3F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x40 - 0x4F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x50 - 0x5F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x60 - 0x6F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x70 - 0x7F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &STA_zp, &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x80 - 0x8F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0x90 - 0x9F
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &LDA_zp, &NOP,    &NOP,    &NOP,    &LDA_i,  &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0xA0 - 0xAF
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0xB0 - 0xBF
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0xC0 - 0xCF
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0xD0 - 0xDF
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0xE0 - 0xEF
	&NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    &NOP,    // 0xF0 - 0xFF

};
