#include "SID.h"
#include "sidwrapper.h"

extern "C"{
	SID* newSID(memtype* mem, int showWindow){
		return new SID(mem, true);
	}

	void* getWindow(SID* sid){
		return sid->getWindow();
	}
	
	void SIDUpdateRegisters(SID* sid){
		sid->updateRegisters();
	}

	void dispatchEvent(SID* sid, void* ev){
		sid->dispatchEvent(ev);
	}


	void SIDDrawGraph(SID* sid){
		sid->drawGraph();
	}

}
