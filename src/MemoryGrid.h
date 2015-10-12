#ifndef MEMORYGRID_H
#define MEMORYGRID_H
#include "SDL.h"
#include <stdio.h>
#include "C64.h"
#include <cmath>
#include <string>
#include "SDL_ttf.h"
#include <stack>

/* Constants */
const int REFRESH_INTERVAL = 2;				// mseconds
const int REPAINTINTERVAL = 200;			// mseconds

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const std::string WINDOW_TITLE = "C64 Memory Window";

class MemoryGrid
{
private:
	C64* theC64;
	SDL_Window *window;          // Declare a pointer
	TTF_Font* font;

	int loopCounter = 0;

	int tilesPerLine;	
	int rectWidth = 1;
	int rectHeight = 1;

	struct ZoomOffset{
		int x = 0;
		int y = 0;

	} zoomOffset;

	struct Tile{
		int x = 0;
		int y = 0;	
	} hoverTile;

	std::stack<ZoomOffset> offsetStack;			// "camera" history

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