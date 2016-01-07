#include "MemoryGrid.h"
#include "memorygridwrapper.h"

extern "C"{
	MemoryGrid* newMemoryGrid(memtype* mem){
		return new MemoryGrid(mem);
	}

	void MemoryGridDrawGrid(MemoryGrid* memgrid){
		memgrid->drawGrid();
	}

}
