#include "MemoryGrid.h"
#include <thread>
#include <iostream>
#include "Utils.h"
#include "CPU.h"

int thread_exit = 0;

int threadFunc(void *pointer){
	MemoryGrid* grid = (MemoryGrid*)pointer;
	grid->init();

	return 0;
}

MemoryGrid::MemoryGrid(C64* c64)
	: theC64(c64), cellsPerLine(256)
{
	// spawn thread
	SDL_Thread *refresh_thread = SDL_CreateThread(threadFunc, NULL, this);
}


MemoryGrid::~MemoryGrid(){

}

void MemoryGrid::init()
{
	// Init SDL & SDL_ttf
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	font = TTF_OpenFont("sans.ttf", 72);	//this opens a font style and sets a size

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"C64 Memory Window",               // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
		);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}
	//SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	mainLoop();
	return;
}

void MemoryGrid::handleZoom(int x, int y, int change){
	// Zoom-levels: 1, 4, 16, 64, 256
	if (cellsPerLine == 256 && change >= 1){
		// max zoom reached
		return;
	}
	else if(change == 0){
		// nothing to do
		return;
	}
	else if (cellsPerLine == 1 && change <= -1){
		// max zoom reached
		return;
	}
		

	int prevCellsPerLine = cellsPerLine;

	if (change >= 1)
		cellsPerLine *= 2;
	else
		cellsPerLine /= 2;

		
	//zoomOffset.x = x / rectWidth;		// previous rectWidth
	//zoomOffset.y = y / rectHeight;		// previous rectHeight		
	if (change < 0){

		offsetStack.push(zoomOffset);

		int distanceX = getCellXAtCoordinates(x, y) - zoomOffset.x;
		if (distanceX % 2 != 0){
			distanceX++;
		}
		int distanceY = getCellYAtCoordinates(x, y) - zoomOffset.y;

		if (distanceY % 2 != 0){
			distanceY++;
		}

		zoomOffset.x += floor((float)distanceX / (float)2);
		zoomOffset.y += floor((float)distanceY / (float)2);
	}
	else{
		ZoomOffset prevOffsetValues = offsetStack.top();
		offsetStack.pop();
		zoomOffset.x = prevOffsetValues.x;
		zoomOffset.y = prevOffsetValues.y;
	}
			

}

void MemoryGrid::mainLoop()
{
	while (thread_exit == 0){
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
			{
				int y = 0;
				break;
			}
			case SDL_MOUSEMOTION:
			{
				SDL_SetWindowTitle(window, Utils::hexify(getCellAtCoordinates(event.motion.x, event.motion.y)).c_str());
				//SDL_UpdateTexture()
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == 1){
					// left click (zoom in)
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
				thread_exit = 1;
				// Close and destroy the window
				SDL_DestroyWindow(window);

				// Clean up
				SDL_Quit();
				return;
				break;
			}
			default: /* unhandled event */
				break;
			}
		}

		drawGrid();
	}


	return;
}

void MemoryGrid::drawGrid()
{
	SDL_Renderer *renderer = SDL_GetRenderer(window);
	if (renderer == nullptr)
		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	// Set background color
	SDL_SetRenderDrawColor(renderer, 88, 88, 88, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	// spanning up grid
	for (int y = 0; y < cellsPerLine; y++){

		for (int x = 0; x < cellsPerLine; x++){
			// Create a rect for each memory cell			
			SDL_Rect r;
			rectWidth	= r.w	= (WINDOW_WIDTH / cellsPerLine);
			rectHeight	= r.h	= (WINDOW_HEIGHT / cellsPerLine);
			r.y			= y * (r.h + 1);		// height + 1 pixel spacing
			r.x			= x * (r.w + 1);		// width + 1 pixel spacing
			rectWidth++;
			rectHeight++;

			// calculate memory address of the cell
			uint16_t cellAddress = ((y + this->zoomOffset.y) << 8) | (x + this->zoomOffset.x);
			uint8_t cellValue = theC64->readMemory(cellAddress);

			// Set colors (Blue = has data, white = no data, red = current PC)		
			if (cellAddress == theC64->getCPU()->Registers.PC){
				SDL_SetRenderDrawColor(renderer, 255, 155, 155, 255);
			}
			else if (cellValue != 0){

				//SDL_SetRenderDrawColor(renderer, 155, 155, 255, 255);
				SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
			}
			else{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			SDL_RenderFillRect(renderer, &r);

			// Render fonts (only if zoomed in because of performance and unreadable texts)			
			switch (cellsPerLine){
				case 256:
				case 128:
				case 64:	// Do not show any text
					break;
				case 32:	// Show address only
				{					

					SDL_Color textColor;
					if (cellValue != 0)			
						textColor = { 222, 222, 222 };
					else
						textColor = { 44, 44, 44 };
					SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, Utils::stringify(cellValue).c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surfaceMessage); // convert it into a texture
					SDL_RenderCopy(renderer, text, NULL, &r);
					SDL_FreeSurface(surfaceMessage);
					SDL_DestroyTexture(text);

					break;
				}
				case 16 :
				case 8:
				case 4:
				case 2:
				case 1:		// Show address and data
				{
					SDL_Rect r1;
					r1.w = rectWidth - 1;
					r1.h = (rectHeight - 1) / 4;
					r1.y = r.y;		// height + 1 pixel spacing
					r1.x = r.x;		// width + 1 pixel spacing			
					SDL_RenderCopy(renderer, NULL, &r1, &r);

					SDL_Rect r2;
					r2.w = r1.w;
					r2.h = r1.h*3;
					r2.y = r1.y + r1.h;		// height + 1 pixel spacing
					r2.x = r1.x;			// width + 1 pixel spacing			
					SDL_RenderCopy(renderer, NULL, &r2, &r);

					SDL_Color textColor;
					if (cellValue != 0)
						textColor = { 222, 222, 222 };
					else
						textColor = { 44, 44, 44 };

					SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, Utils::hexify(cellAddress).c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_Texture* addressText = SDL_CreateTextureFromSurface(renderer, surfaceMessage); // convert it into a texture
					SDL_RenderCopy(renderer, addressText, NULL, &r1);
					SDL_FreeSurface(surfaceMessage);
					SDL_DestroyTexture(addressText);


					//SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, Utils::hexify(cellAddress).c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, Utils::stringify(cellValue).c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_Texture* valueText = SDL_CreateTextureFromSurface(renderer, surfaceMessage2); // convert it into a texture
					SDL_RenderCopy(renderer, valueText, NULL, &r2);
					SDL_FreeSurface(surfaceMessage2);
					SDL_DestroyTexture(valueText);

					break;
				}

			}
				
		}				
		
	}

	SDL_RenderPresent(renderer);
	
	SDL_Delay(REFRESH_INTERVAL);

	return;
}

uint16_t MemoryGrid::getCellAtCoordinates(int x, int y){
	uint8_t lowByte = zoomOffset.x + (x / rectWidth);
	uint8_t highByte = zoomOffset.y + (y / rectHeight);
	return Utils::makeWord(lowByte, highByte);
}

int MemoryGrid::getCellXAtCoordinates(int x, int y){
	uint8_t lowByte = zoomOffset.x + (x / rectWidth);

	return lowByte;
}


int MemoryGrid::getCellYAtCoordinates(int x, int y){
	uint8_t highByte = zoomOffset.y + (y / rectHeight);

	return highByte;
}