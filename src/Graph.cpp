#include "Graph.h"
#include <thread>
#include <iostream>
#include <sstream>
#include <string>

int graphThreadFunc(void *pointer){
	Graph* grid = (Graph*)pointer;
	grid->init();

	return 0;
}

Graph::Graph(uint8_t* buffer)
	: tilesPerLine(256), buffer(buffer)
{
	//memcpy(buffer, buffer, 44100);
	// spawn thread
	SDL_Thread *refresh_thread = SDL_CreateThread(graphThreadFunc, NULL, this);
}



void Graph::init()
{
	// Init SDL & SDL_ttf
	SDL_Init(SDL_INIT_VIDEO);

	// Settings
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		WINDOW_TITLE.c_str(),  			   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		SDL_WINDOW_SHOWN                  // flags - see below
		);

	// Check if the window was successfully created
	if (window == NULL) {
		// In case the window could not be created...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}
	else{
		mainLoop();
		return;
	}
}

void Graph::handleZoom(int x, int y, int zoom){
	// tilesPerLine indicates how many tiles are shown. It currently supports the zoom-levels 2^1 to 2^8:  256, 128, 64, 32, 16, 4, 2, 1
	if (tilesPerLine == 256 && zoom >= 1){
		// Zoom out of boundaries
		return;
	}
	else if (zoom == 0){
		// No change (initial zoom)
		return;
	}
	else if (tilesPerLine == 1 && zoom <= -1){
		// Zoom out of boundaries
		return;
	}

	// remember the old (=current) "camera" settings
	int prevTilesPerLine = tilesPerLine;

	// adjust new grid size
	if (zoom >= 1)
		tilesPerLine *= 2;
	else
		tilesPerLine /= 2;


	if (zoom < 0){
		// If we are zooming in...
		offsetStack.push(zoomOffset);			// remember history of camera settings

		int distanceX = getCellXAtCoordinates(x, y) - zoomOffset.x;
		if (distanceX % 2 != 0){				// make sure distance is an even number
			distanceX++;
		}
		int distanceY = getCellYAtCoordinates(x, y) - zoomOffset.y;

		if (distanceY % 2 != 0){				// make sure distance is an even number
			distanceY++;
		}

		// calculate new offset
		zoomOffset.x += floor((float)distanceX / (float)2);
		zoomOffset.y += floor((float)distanceY / (float)2);
	}
	else{
		// Zoom out: Restore camera history from stack
		ZoomOffset prevOffsetValues = offsetStack.top();
		offsetStack.pop();
		zoomOffset.x = prevOffsetValues.x;
		zoomOffset.y = prevOffsetValues.y;
	}


}

void Graph::mainLoop()
{
	while (thread_exit == 0){
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
					pause_thread = !pause_thread;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
					exit();
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN){
					if (pause_thread){
						// submit
						//theC64->writeMemory(getCellAtCoordinates(hoverTile.x, hoverTile.y), inputBuffer);
						drawGrid();
					}
				}
				else{
					if (pause_thread){
						inputBuffer = event.key.keysym.sym;

					}
				}

				break;
			}
			case SDL_MOUSEMOTION:
			{
				
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{

				if (event.button.button == 1){
					// left click (zoom in))
					int xcoord = event.button.x;
					int ycoord = event.button.y;
					handleZoom(xcoord, ycoord, -1);
				}
				else if (event.button.button == 3){
					// right click (zoom out)
					int xcoord = event.button.x;
					int ycoord = event.button.y;
					handleZoom(xcoord, ycoord, +1);
				}
				else{
					// unhandled button
				}
				break;
			}
			case SDL_QUIT:
			{
				exit();
				return;
				break;
			}
			default: /* unhandled event */
				break;
			}
		}

		loopCounter += 2;
		if (loopCounter % REPAINTINTERVAL == 0 && !pause_thread)
			drawGrid();
	}


	return;
}

void Graph::drawGrid()
{
	SDL_Renderer *renderer = SDL_GetRenderer(window);
	if (renderer == nullptr)
		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	// Set background color
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	/*float RMS = 0;
	int BlockSize = 2000 / WINDOW_WIDTH;
	for (int x = 0; x < WINDOW_WIDTH; x++){
		for (int a = 0; a < BlockSize; a++)
		{
			RMS += buffer[a + x*BlockSize] * buffer[a + x*BlockSize];
		}
		RMS = sqrt(RMS / BlockSize);

		SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);


		if (x != 0 && RMS != 0)
			SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT-RMS);
	}*/

	SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
	for (int x = 0; x < WINDOW_WIDTH; x++){
		uint8_t y = buffer[x];
		SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT - y);
	}
	

	SDL_RenderPresent(renderer);

	SDL_Delay(REFRESH_INTERVAL);

	return;
}

uint16_t Graph::getCellAtCoordinates(int x, int y){
	uint8_t lowByte = zoomOffset.x + (x / rectWidth);
	uint8_t highByte = zoomOffset.y + (y / rectHeight);
	return (highByte << 8 + lowByte);
}

int Graph::getCellXAtCoordinates(int x, int y){
	uint8_t lowByte = zoomOffset.x + (x / rectWidth);

	return lowByte;
}


int Graph::getCellYAtCoordinates(int x, int y){
	uint8_t highByte = zoomOffset.y + (y / rectHeight);

	return highByte;
}

void Graph::exit(){
	thread_exit = 1;
	// Close and destroy the window
	SDL_DestroyWindow(window);
	// Clean up
	SDL_Quit();
}