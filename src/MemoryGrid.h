#ifndef MEMORYGRID_H
#define MEMORYGRID_H
#include "SDL.h"
#include <stdio.h>
#include "C64.h"
#include <cmath>
#include <string>
#include "SDL_ttf.h"
#include <stack>


const int REFRESH_INTERVAL = 100;				// mseconds
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

class MemoryGrid
{
private:
	C64* theC64;
	SDL_Window *window;          // Declare a pointer
	TTF_Font* font;

	int cellsPerLine;
	
	int rectWidth = 1;
	int rectHeight = 1;

	int quadrant = 0;

	struct ZoomOffset{
		int x = 0;
		int y = 0;

	} zoomOffset;

	std::stack<ZoomOffset> offsetStack;
	


public:
	MemoryGrid(C64* theC64);
	~MemoryGrid();
	void init();
	void mainLoop();
	void drawGrid();
	void handleZoom(int x, int y, int change);
	uint16_t getCellAtCoordinates(int x, int y);
	int getCellXAtCoordinates(int x, int y);
	int getCellYAtCoordinates(int x, int y);

};


#endif