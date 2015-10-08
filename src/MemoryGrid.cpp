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
	SDL_Thread *refresh_thread = SDL_CreateThread(threadFunc, NULL, this);
}


MemoryGrid::~MemoryGrid(){
	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
}

void MemoryGrid::init()
{
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	TTF_Init();
	font = TTF_OpenFont("OpenSans.ttf", 22); //this opens a font style and sets a size

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
	if (cellsPerLine == 256 && change >= 1){
		// cannot zoom further out
	}
	else if(change == 0){
		// nothing to do
	}
	else if (change == 4 && change <= -1 ){
		// nothing to do
	}
	else{
		zoomPivot.x = x;
		zoomPivot.y = y;
		if (change >= 1)
			cellsPerLine *= 2;
		else
			cellsPerLine /= 2;
	}
}

void MemoryGrid::drawGrid()
{
	while (thread_exit == 0){
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// handle your event here
			switch (event.type)
			{  /* Schau nach, welcher Event eingetroffen ist */
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
				/* Mausevent */
				break;
			}
			case SDL_QUIT:
			{
				thread_exit = 1;
				this->~MemoryGrid();
				return;
				break;
			}
			default: /* unbeachteter Event */
				break;
			}
		}

		// handle your event here
		// Setup renderer
		SDL_Renderer *renderer = SDL_GetRenderer(window);
		if (renderer == nullptr)
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);


		// Set render color to black ( background will be rendered in this color )
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		// Clear winow
		SDL_RenderClear(renderer);

		int xpos = 0;

		for (int i = 0; i <cellsPerLine; i++){

			for (int j = 0; j < cellsPerLine; j++){
				// Create a rect for each memory cell			
				SDL_Rect r;
				r.w = WINDOW_WIDTH / cellsPerLine;
				r.h = WINDOW_HEIGHT / cellsPerLine;
				r.y = i * r.h + 1*i;
				r.x = j * r.w + 1 * j;

				uint16_t cellAddress = (i << 8) | j;

				// Set render color to white ( rect will be rendered in this color )			
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

				// Render rect			
				if (cellsPerLine <= 64){

					SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
					SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, Utils::hexify(cellAddress).c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

					SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

					SDL_RenderCopy(renderer, text, NULL, &r);
				}
			}				
		
		}

		SDL_RenderPresent(renderer);		
		SDL_Delay(REFRESH_INTERVAL);
	}


	return;
}