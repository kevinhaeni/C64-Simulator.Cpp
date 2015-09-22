#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <string>

class Memory
{
private:
	uint8_t *_mem;
	uint16_t memsize;

public:	

	// Construction
	Memory(uint16_t size);		
	void reset();

	// Memory access
	uint8_t read_byte(uint16_t adr);
	void write_byte(uint16_t adr, uint8_t val);

	// Helper methods
	void dump(std::string filePath, bool showAdr);
	void save(std::string filePath);
	void load(std::string filePath);

};

#endif