#ifndef C64_H
#define C64_H
#include <cstdint>
#include "Memory.h"
#include "../CPU/CPU.h"

class C64
{
private:
	Memory *ram;
	CPU *cpu;

public:
	C64();

	void doSomeThings();
};

#endif