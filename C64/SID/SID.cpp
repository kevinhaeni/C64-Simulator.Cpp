#include "SID.h"

SID::SID()
{

}

uint8_t SID::read_byte(uint16_t adr){
	return regs[adr];
}

void SID::write_byte(uint16_t adr, uint8_t data){
	regs[adr] = data;
}

void SID::emulateCycles(){

}
