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
	font = TTF_OpenFont("OpenSans.ttf", 22);	//this opens a font style and sets a size

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
	drawGrid();
	return;
}

void MemoryGrid::handleZoom(int x, int y, int change){
	// Zoom-levels: 1, 4, 16, 64, 256
	if (cellsPerLine == 256 && change >= 1){
		// max zoom reached
	}
	else if(change == 0){
		// nothing to do
	}
	else if (cellsPerLine == 1 && change <= -1){
		// max zoom reached
	}
	else{	

		int prevCellsPerLine = cellsPerLine;

		if (change >= 1)
			cellsPerLine *= 2;
		else
			cellsPerLine /= 2;

		if (cellsPerLine == 256){
			quadrant = 0;
		}else{
			quadrant = 1;
			if (x > WINDOW_WIDTH / 2)
				quadrant += 1;
			if (y > WINDOW_HEIGHT / 2)
				quadrant += 2;
		}
		
		//zoomOffset.x = x / rectWidth;		// previous rectWidth
		//zoomOffset.y = y / rectHeight;		// previous rectHeight		
		if (change < 0){

			offsetStack.push(zoomOffset);

			switch (quadrant){
			case 1:
			{
				//zoomOffset.x = zoomOffset.x;
				break;
			}
			case 2:
			{
				zoomOffset.x = zoomOffset.x - change * (prevCellsPerLine / 2);
				break;
			}
			case 3:
			{
				zoomOffset.y = zoomOffset.y - change * (prevCellsPerLine / 2);
				break;
			}
			case 4:
			{
				zoomOffset.x = zoomOffset.x - change * (prevCellsPerLine / 2);
				zoomOffset.y = zoomOffset.y - change * (prevCellsPerLine / 2);
				break;
			}
			default:
				zoomOffset.x = 0;
				zoomOffset.y = 0;
			};			
		}
		else{
			ZoomOffset prevOffsetValues = offsetStack.top();
			offsetStack.pop();
			zoomOffset.x = prevOffsetValues.x;
			zoomOffset.y = prevOffsetValues.y;
		}
			
	}
}

void MemoryGrid::drawGrid()
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

		// handle your event here
		// Setup renderer
		SDL_Renderer *renderer = SDL_GetRenderer(window);
		if (renderer == nullptr)
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

		// Set background color
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		// Clear winow
		SDL_RenderClear(renderer);

		// spanning up grid
		for (int y = 0; y < cellsPerLine; y++){

			for (int x = 0; x < cellsPerLine; x++){
				// Create a rect for each memory cell			
				SDL_Rect r;
				rectWidth	= r.w	= (WINDOW_WIDTH / cellsPerLine);
				rectHeight	= r.h	= (WINDOW_HEIGHT / cellsPerLine);
				r.y			= y * (r.h + 1);		// width + 1 pixel spacing
				r.x			= x * (r.w + 1);		// height + 1 pixel spacing

				// calculate memory address of the cell
				uint16_t cellAddress = ((y + this->zoomOffset.y) << 8) | (x + this->zoomOffset.x);

				// Set colors (Blue = has data, white = no data, red = current PC)		
				if (cellAddress == theC64->getCPU()->Registers.PC){
					SDL_SetRenderDrawColor(renderer, 255, 155, 155, 255);
				}
				else if (theC64->readMemory(cellAddress) != 0){

					SDL_SetRenderDrawColor(renderer, 155, 155, 255, 255);
				}
				else{
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				SDL_RenderFillRect(renderer, &r);

				// Render fonts (only if zoomed in because of performance and unreadable texts)			
				if (cellsPerLine <= 64){

					SDL_Color White = { 255, 255, 255 }; 
					SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, Utils::hexify(cellAddress).c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

					SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surfaceMessage); // convert it into a texture

					SDL_RenderCopy(renderer, text, NULL, &r);

					// free texture memory
					SDL_FreeSurface(surfaceMessage);
					SDL_DestroyTexture(text);					
					
				}
				
			}				
		
		}

		SDL_RenderPresent(renderer);
	
		SDL_Delay(REFRESH_INTERVAL);
	}


	return;
}