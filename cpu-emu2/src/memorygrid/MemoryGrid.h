#ifndef MEMORYGRID_H
#define MEMORYGRID_H
#include "SDL.h"
#include <string>
#include "SDL_ttf.h"
#include <stack>
#include "Utils.h"
#include <sstream>
#include <string>

typedef char memory[0x10000][9];


/* Constants */
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const std::string WINDOW_TITLE = "C64 Memory Window";

void mainLoop();

class MemoryGrid
{
public:

	SDL_Window *window;          // Declare a pointer
	SDL_Renderer *renderer;
	TTF_Font* font;

	int thread_exit = 0;
	
	void dispatchEvent(SDL_Event* event);
	memory* _mem;

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
	
	char inputBuffer[2];
	int inputBufferPos = 0;

	std::stack<ZoomOffset> offsetStack;			// "camera" history


	MemoryGrid(memory* mem);
	~MemoryGrid();
	void init();
	
	void drawGrid();
	void handleZoom(int x, int y, int change);
	uint16_t getCellAtCoordinates(int x, int y);
	int getCellXAtCoordinates(int x, int y);
	int getCellYAtCoordinates(int x, int y);
	void exit();

	uint8_t readMemory(uint16_t addr) const;
	void writeMemory(uint8_t value, uint16_t addr) const;

	bool pause_thread = false;

};


#endif
