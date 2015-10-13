#ifndef FILTER_H
#define FILTER_H


/*
Filter Class
*/
class Filter
{
protected:
public:
	Filter();
    
    void write_FC_LO(uint8_t value);
    void write_FC_HI(uint8_t value);
    void write_RES_FILT(uint8_t value);
    void write_MODE_VOL(uint8_t value);
};

#endif