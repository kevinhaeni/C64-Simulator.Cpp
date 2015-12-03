#include "SID.h"
#include "sidwrapper.h"

extern "C"{
	SID* newSID(memory* mem, int interval, bool showWindow){
		return new SID(mem, 50, true);
	}

}
