#include "MemoryGrid.h"
#include <thread>
#include <iostream>
#include "Utils.h"

int thread_exit = 0;

int threadFunc(void *pointer){
	MemoryGrid* grid = (MemoryGrid*)pointer;
	grid->init();

	return 0;
}

MemoryGrid::MemoryGrid(C64* c64)
	: theC64(c64), zoom(1)
{
	numOfElements = MEMSIZE;					// get the number of memory cells
	lineSize = std::sqrt(numOfElements);		// squareroot to get the number of rows and columns

	int data = 10;
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
	font = TTF_OpenFont("OpenSans.ttf", 48); //this opens a font style and sets a size

	int window_width, window_height = 0;

	window_width = (lineSize * (RECT_WIDTH + RECT_SPACING)) + 2 * BORDER_WIDTH;
	window_height = (lineSize * (RECT_WIDTH + RECT_SPACING)) + 2 * BORDER_WIDTH;


	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		window_width,                      // width, in pixels
		window_height,                     // height, in pixels
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
	if (zoom == 1 && change <= -1){
		// cannot zoom further out
	}
	else if(change == 0){
		// nothing to do
	}
	else{
		zoomPivot.x = x;
		zoomPivot.y = y;
		zoom += change;
	}
}

SDL_Texture* MemoryGrid::createText(std::string text, SDL_Renderer* renderer){
	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size

	SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
	
	return Message;
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
					handleZoom(xcoord, ycoord, +10);
				}
				else if (event.button.button == 3){
					// right click (zoom out)
					int xcoord = event.button.x;
					int ycoord = event.button.y;
					handleZoom(xcoord, ycoord, -10);
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

		int xpos = BORDER_WIDTH + RECT_SPACING;						// set the margin/spacing between each square

		for (uint16_t i = 0; i < (uint16_t)MEMSIZE; i++){
			// Creat a rect for each memory cell			
			SDL_Rect r;

			if (zoom == 1){
				// No zoom, we show all cells

				//r.x = 5 + (RECT_WIDTH*i) + i*5;
				if (i % lineSize == 0){
					xpos = BORDER_WIDTH + RECT_SPACING;
				}
				else{
					xpos += RECT_WIDTH + RECT_SPACING;
				}
				r.x = xpos;
				r.y = BORDER_WIDTH + RECT_SPACING + (i / lineSize)*RECT_WIDTH + (i / lineSize) * RECT_SPACING;
				r.w = RECT_WIDTH;
				r.h = RECT_WIDTH;
			}
			else{
				// With zoom
				//r.x = 5 + (RECT_WIDTH*i) + i*5;
				if (i % lineSize == 0){
					xpos = BORDER_WIDTH + RECT_SPACING;
				}
				else{
					xpos += RECT_WIDTH*zoom + RECT_SPACING;
				}
				r.x = xpos;
				r.y = BORDER_WIDTH + RECT_SPACING + (i / lineSize)*RECT_WIDTH*zoom + (i / lineSize) * RECT_SPACING;
				r.w = RECT_WIDTH*zoom;
				r.h = RECT_WIDTH*zoom;
			}			

			
			// Set render color to white ( rect will be rendered in this color )			
			if (theC64->readMemory(i) != 0){

				SDL_SetRenderDrawColor(renderer, 155, 155, 255, 255);
			}
			else{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			SDL_RenderFillRect(renderer, &r);

			// Render rect			
			if (zoom >= 10 && i <= 1000){				

				SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
				SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, Utils::hexify(i).c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

				SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

				SDL_RenderCopy(renderer, text, NULL, &r);
			}
			
		
		}

		SDL_RenderPresent(renderer);		
		SDL_Delay(REFRESH_INTERVAL);
	}


	return;
}