#ifndef SID_H
#define SID_H
#include <cstdint>

#include "Voice.h"


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

    //ExternalFilter extfilter;
    //Potentiometer potx;
    //Potentiometer poty;
    
public:
    
    /*
     Filter Class
     */
    struct Filter
    {
    public:
        Filter();
        
        void write_FC_LO(byte value);
        void write_FC_HI(byte value);
        void write_RES_FILT(byte value);
        void write_MODE_VOL(byte value);
    }Filter;

	SID();

	byte read_byte(word adr);
	void write_byte(word adr, byte data);

    void updateRegisters();
	void emulateCycles();
};

#endif