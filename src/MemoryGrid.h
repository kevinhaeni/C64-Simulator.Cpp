#ifndef MEMORYGRID_H
#define MEMORYGRID_H
#include "SDL.h"
#include <stdio.h>
#include "C64.h"
#include <cmath>
#include <string>
#include "SDL_ttf.h"


const int REFRESH_INTERVAL = 600;				// mseconds
const int DEFAULT_WINDOW_WIDTH = 1540;
const int DEFAULT_WINDOW_HEIGHT = 800;

const int BORDER_WIDTH = 10;
const int RECT_WIDTH = 2;
const int RECT_SPACING = 1;

class MemoryGrid
{
private:
	C64* theC64;
	SDL_Window *window;          // Declare a pointer
	TTF_Font* font;

	int numOfElements;
	int lineSize;
	int zoom = 1;
	struct ZoomPivot{
		int x;
		int y;
	} zoomPivot;


public:
	MemoryGrid(C64* theC64);
	~MemoryGrid();
	void init();
	void drawGrid();
	void handleZoom(int x, int y, int change);
	SDL_Texture* createText(std::string text, SDL_Renderer* renderer);
};


#endif