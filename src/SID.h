#ifndef SID_H
#define SID_H
#include <cstdint>

#include "Voice.h"
#include "Filter.h"

/*
SID Class
*/
class SID
{
private:
	uint8_t regs[29];

    Voice voice1;
    Voice voice2;
    Voice voice3;
    Filter filter;
    //ExternalFilter extfilter;
    //Potentiometer potx;
    //Potentiometer poty;
    
public:

	SID();

	uint8_t read_byte(uint16_t adr);
	void write_byte(uint16_t adr, uint8_t data);

    void updateRegisters();
	void emulateCycles();
};

#endif