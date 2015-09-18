#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>

using namespace std;

const int MEMSIZE = 65536;


class Memory
{
private:
	uint8_t mem[MEMSIZE];

public:
	// ctor
	Memory();
	void reset();

	uint8_t read(uint16_t adr);
	void write(uint16_t adr, uint8_t val);

};

#endif