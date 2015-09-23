#ifndef SID_H
#define SID_H
#include <cstdint>


/*
SID Class
*/
class SID
{
private:
	uint8_t regs[29];

public:

	SID();

	uint8_t read_byte(uint16_t adr);
	void write_byte(uint16_t adr, uint8_t data);

	void emulateCycles();
};

#endif