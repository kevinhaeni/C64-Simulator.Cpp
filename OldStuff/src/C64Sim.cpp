// C64.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include "C64.h"
#include "SDL.h"
#include "MemoryGrid.h"
#include "Utils.h"

#include <thread>
#include "SID.h"

#define DEBUG 0

MemoryGrid* memGrid;
SID* sid;
memory* _mem;

// Reads the value at the given memory address
// also converts the [9] bitwise representation (e.g.: "01011101\0") into a unsigned decimal number
uint8_t readMemory(uint16_t addr)
{
	uint8_t val = 0;
	for (int i = 7; i >= 0; i--){
		if ((*_mem)[addr][i] == '1'){
			val += pow(2, 7 - i);
		}
	}
	return val;
}

// Writes the value to the given memory address
void writeMemory(uint8_t value, uint16_t addr){

	//std::cout <<  "Write Memory" << std::endl << "Before: " << readMemory(addr) << std::endl;
	for (int i = 7; i >= 0; i--){
		if (value & 0x01){
			(*_mem)[addr][i] = '1';
		}
		else{
			(*_mem)[addr][i] = '0';
		}
		value >>= 1;
	}
	//std::cout <<  "After: " << readMemory(addr) << std::endl;
}

int main(int argc, char* argv[]){

	char memory[0x10000][9];		// the memory
	_mem = &memory;

	memory[0x0001][0] = '1';
	memory[0x0001][1] = '1';
	memory[0x0001][2] = '1';
	memory[0x0001][3] = '1';
	memory[0x0001][4] = '1';
	memory[0x0001][5] = '1';
	memory[0x0001][6] = '1';
	memory[0x0001][7] = '1';


	memory[0xD400][0] = '1';
	memory[0xD400][1] = '1';
	memory[0xD400][2] = '1';
	memory[0xD400][3] = '1';
	memory[0xD400][4] = '1';
	memory[0xD400][5] = '1';
	memory[0xD400][6] = '1';
	memory[0xD400][7] = '1';


	memory[0xD401][0] = '0';
	memory[0xD401][1] = '0';
	memory[0xD401][2] = '0';
	memory[0xD401][3] = '0';
	memory[0xD401][4] = '0';
	memory[0xD401][5] = '0';
	memory[0xD401][6] = '0';
	memory[0xD401][7] = '0';

	// Init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	TTF_Init();

	// Instantiate SID and memory grid
	memGrid = new MemoryGrid(&memory);
	sid = new SID(&memory, true);



	int i;
	std::cin >> i;
	return 0;
}