#include "SID.h"
#include "sidwrapper.h"

extern "C"{
	SID* newSID(memtype* mem, int interval, int showWindow){
		return new SID(mem, 50, true);
	}

	void* SID::getWindow(SID* sid){
		return sid->getWindow();
	}
}
