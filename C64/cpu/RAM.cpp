#include "stdafx.h"
#include "Memory.h"


/*
Constructor
*/
Memory::Memory(){
	this->reset();

}

void Memory::reset(){
	for (int i = 0; i <= MEMSIZE; i++){
		mem[i] = 0x00;
	}
}

uint8_t Memory::read(uint16_t adr){

}

void Memory::write(uint16_t adr, uint8_t val){

}
