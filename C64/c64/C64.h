#ifndef C64_H
#define C64_H
#include <cstdint>
#include "Memory.h"
#include "../CPU/CPU.h"
#include "../SID/SID.h"

class C64
{
private:
	CPU cpu;
	SID *sid;

public:
	C64();

	void run();
	void reset();

	void test();
};

#endif