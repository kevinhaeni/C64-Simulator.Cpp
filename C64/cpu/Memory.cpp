#include "Memory.h"
#include "../util/Utils.h"
#include <exception>
#include <fstream>

/*
	Constructor
*/
Memory::Memory(uint16_t size){
	this->memsize = size;
	this->_mem = new uint8_t[size];
	
	this->reset();	
}

/*
	Set every byte to 0x00
*/
void Memory::reset(){
	for (uint16_t i = 0; i < this->memsize; i++){
		_mem[i] = 0x00;
	}
}

/*
	Fetch data
*/
uint8_t Memory::read_byte(uint16_t adr){
	if (adr <= this->memsize && adr >= 0)
		return _mem[adr];
	else
		throw std::exception("Invalid memory address");
}


/*
	Write data
*/
void Memory::write_byte(uint16_t adr, uint8_t val){
	if (adr <= this->memsize && adr >= 0)
		_mem[adr] = val;
	else
		throw std::exception("Invalid memory address");
}


void Memory::dump(std::string filePath, bool showAdr){
	// write memory content to file
	std::cout << "Dump memory...";

	std::ofstream myfile;
	myfile.open(filePath);
	for (uint16_t adr = 0; adr < this->memsize; adr++){
		if (showAdr){
			myfile << "[" << Utils::hexify(adr) << "] = " << Utils::hexify(_mem[adr]) << std::endl;
		}
		else{
			myfile << Utils::hexify(_mem[adr]);
		}
	}	
	myfile.close();
}
