#include "SID.h"
#include "sidwrapper.h"

extern "C"{
	SID* newSID(memtype* mem, int showWindow){
		return new SID(mem, true);
	}


}
