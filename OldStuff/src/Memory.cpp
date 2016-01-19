#include "Memory.h"
#include "Utils.h"
#include <fstream>
#include <vector>

/*
	Constructor
*/
Memory::Memory(uint16_t size){
	this->memsize = size;
	this->_mem = new uint8_t[size];
	
	this->reset();	
}

/*
	Reset: Set every byte to 0x00
*/
void Memory::reset(){
	for (uint16_t i = 0; i < this->memsize; i++){
		_mem[i] = 0x00;
	}
}


/*
	Memory Access (read)
*/
uint8_t Memory::read_byte(uint16_t adr){
	return _mem[adr];
}


/*
	Memory Access (write)
*/
void Memory::write_byte(uint16_t adr, uint8_t val){
	_mem[adr] = val;
}


void Memory::dump(std::string filePath, bool showAdr){
	// write memory content to file

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


void Memory::save(std::string filePath){
	// write memory content to file
	std::cout << "Save memory...";

	std::ofstream myfile;
	myfile.open(filePath);
	for (uint16_t adr = 0; adr < this->memsize; adr++){
	
		myfile << _mem[adr];
	}
	myfile.close();
}


void Memory::load(std::string filePath){
	// load memory from file
	std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	for (uint16_t adr = 0; adr < this->memsize; adr++){

		_mem[adr] = result[adr];
	}
}

void Memory::load(std::string filePath, uint16_t from, uint16_t size){
	// load memory from file
	std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	for (uint16_t i = 0; i < size; i++){

		_mem[from + i] = result[i];
	}
}
