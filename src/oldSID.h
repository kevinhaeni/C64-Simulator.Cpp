#ifndef oldSID_H
#define oldSID_H
#include <cstdint>
#include <iostream>


typedef uint8_t byte;
typedef uint16_t word;

/*
SID Class
*/
class oldSID
{
private:
    
    
    
    
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

	oldSID();
    
    
	byte read_byte(word adr);
	void write_byte(word adr, byte data);

    void updateRegisters();
	void emulateCycles(int cyclesTodo);
    void emulateCycle();
};

#endif