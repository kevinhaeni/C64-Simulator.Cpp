#ifndef GRAPH_H
#define GRAPH_H
#include "SDL.h"
#include <stdio.h>
#include <cmath>
#include <string>
#include <stack>

/* Constants */
const int REFRESH_INTERVAL = 2;				// mseconds
const int REPAINTINTERVAL = 200;			// mseconds

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 255;
const std::string WINDOW_TITLE = "Wave Graph";

class Graph
{
private:
	SDL_Window *window;          // Declare a pointer

	int thread_exit = 0;
	bool pause_thread = false;

	uint8_t* buffer;

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

	int inputMode = 0;
	uint8_t inputBuffer;
	std::stack<ZoomOffset> offsetStack;			// "camera" history

public:
	Graph(uint8_t* buffer);
	void init();
	void mainLoop();
	void drawGrid();
	void handleZoom(int x, int y, int change);
	uint16_t getCellAtCoordinates(int x, int y);
	int getCellXAtCoordinates(int x, int y);
	int getCellYAtCoordinates(int x, int y);
	void exit();

};


#endif