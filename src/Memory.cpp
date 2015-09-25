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

	rw.clear();
	dataBus = 0x00;
	addressBus = 0x00;
}

void Memory::memoryAccess(){
	if (rw.get()){
		if (addressBus <= this->memsize && addressBus >= 0)
			_mem[addressBus] = dataBus;
		else {
			//throw std::exception("Invalid memory address");
		}			
	}
	else{
		// read access
		if (addressBus <= this->memsize && addressBus >= 0)
			dataBus = _mem[addressBus];
		else {
			//throw std::exception("Invalid memory address");
		}
			
	}
}

/*
	Memory Access (read)
*/
uint8_t Memory::read_byte(uint16_t adr){
	addressBus = adr;
	rw.clear();
	memoryAccess();
	return dataBus;
}


/*
	Memory Access (write)
*/
void Memory::write_byte(uint16_t adr, uint8_t val){
	addressBus = adr;	
	dataBus = val;
	rw.set();
	memoryAccess();
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
	// write memory content to file
	std::cout << "Load memory...";
	std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	for (uint16_t adr = 0; adr < this->memsize; adr++){

		_mem[adr] = result[adr];
	}
}
