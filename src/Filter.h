#ifndef FILTER_H
#define FILTER_H
#include <cstdint>

typedef uint8_t byte;
typedef uint16_t word;

/*
Filter Class
*/
class Filter
{
protected:
public:
	Filter();
    
    void write_FC_LO(byte value);
    void write_FC_HI(byte value);
    void write_RES_FILT(byte value);
    void write_MODE_VOL(byte value);
};

#endif