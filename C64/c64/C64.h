#ifndef C64_H
#define C64_H
#include <cstdint>
#include "Memory.h"
#include "../CPU/CPU.h"
#include "../SID/SID.h"


	class C64
	{
	private:
		Memory *mem;
		CPU *cpu;
		SID *sid;

	public:
		C64();

		uint8_t Read(int adr);

		void run();
		void reset();

		void test();
	};


#endif