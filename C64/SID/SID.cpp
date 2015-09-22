#include "SID.h"

SID::SID()
{

}

uint8_t SID::read_byte(uint16_t adr){
	return regs[adr];
}