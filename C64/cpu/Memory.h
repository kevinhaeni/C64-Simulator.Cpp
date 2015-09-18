#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <string>

class Memory
{
private:
	uint8_t *_mem;	

public:
	uint16_t memsize;

	// ctor
	Memory(uint16_t size);
	void reset();

	uint8_t read_byte(uint16_t adr);
	void write_byte(uint16_t adr, uint8_t val);

	void dump(std::string filePath, bool showAdr);

};

#endif