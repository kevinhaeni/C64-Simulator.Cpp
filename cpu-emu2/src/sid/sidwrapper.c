#include "Graph.h"
#include "sidwrapper.h"

extern "C"{
	Graph* newGraph(){
		return new Graph();
	}

}
