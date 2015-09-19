#ifndef SID_H
#define SID_H
#include <cstdint>
#include "../CPU/CPU.h"

/*
SID Class
*/
class SID
{
private:
	CPU *cpu;

public:

	SID(CPU* cpu);
};

#endif