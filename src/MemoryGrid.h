#ifndef MEMORYGRID_H
#define MEMORYGRID_H
#include "SDL.h"
#include <stdio.h>
#include "C64.h"
#include <cmath>
#include <string>
#include "SDL_ttf.h"


const int REFRESH_INTERVAL = 600;				// mseconds
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

class MemoryGrid
{
private:
	C64* theC64;
	SDL_Window *window;          // Declare a pointer
	TTF_Font* font;

	int cellsPerLine;
	
	int rectWidth = 0;
	int rectHeight = 0;

	struct ZoomOffset{
		int x;
		int y;
	} zoomOffset;


public:
	MemoryGrid(C64* theC64);
	~MemoryGrid();
	void init();
	void drawGrid();
	void handleZoom(int x, int y, int change);
};


#endif