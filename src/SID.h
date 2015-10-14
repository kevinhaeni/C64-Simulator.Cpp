#ifndef SID_H
#define SID_H
#include <cstdint>

#include "Voice.h"
#include "Filter.h"


typedef uint8_t byte;
typedef uint16_t word;

/*
SID Class
*/
class SID
{
private:
    Voice voice1;
    Voice voice2;
    Voice voice3;
    Filter filter;
    //ExternalFilter extfilter;
    //Potentiometer potx;
    //Potentiometer poty;
    
public:

	SID();

	byte read_byte(word adr);
	void write_byte(word adr, byte data);

    void updateRegisters();
	void emulateCycles();
};

#endif