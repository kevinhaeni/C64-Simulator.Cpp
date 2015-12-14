#include "MemoryGrid.h"
#include "Utils.h"
#include "CPU.h"
#include <sstream>
#include <string>



int threadFunc(void *pointer){
	MemoryGrid* grid = (MemoryGrid*)pointer;
	grid->init();

	return 0;
}

MemoryGrid::MemoryGrid(memory* mem)
	: _mem(mem), tilesPerLine(256)
{
	// spawn thread
	SDL_Thread *refresh_thread = SDL_CreateThread(threadFunc, NULL, this);
}


MemoryGrid::~MemoryGrid(){

}

int main(int argc, char* argv[]){

	char memory[0x10000][9];		// the memory
	memory[0xD400][0] = 1;
	memory[0xD400][1] = 1;
	memory[0xD400][2] = 1;
	memory[0xD400][3] = 1;
	memory[0xD400][4] = 1;
	memory[0xD400][5] = 1;
	memory[0xD400][6] = 1;
	memory[0xD400][7] = 1;


	memory[0xD401][0] = 0;
	memory[0xD401][1] = 0;
	memory[0xD401][2] = 0;
	memory[0xD401][3] = 0;
	memory[0xD401][4] = 0;
	memory[0xD401][5] = 0;
	memory[0xD401][6] = 0;
	memory[0xD401][7] = 0;

	memory[0xD404][0] = 0;
	//memory[0xD404][1] = ;
	//memory[0xD404][2] = ;
	memory[0xD404][3] = 1;
	memory[0xD404][4] = 0;
	memory[0xD404][5] = 1;
	memory[0xD404][6] = 0;
	memory[0xD404][7] = 0;

	MemoryGrid* g = new MemoryGrid(&memory);

	int i;
	std::cin >> i;
	return 0;
}


void MemoryGrid::init()
{
	// Init SDL & SDL_ttf
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	
	// Settings
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	font = TTF_OpenFont("sans.ttf", 158);	//this opens a font style and sets a size

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

void MemoryGrid::handleZoom(int x, int y, int zoom){
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

void MemoryGrid::mainLoop()
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
						theC64->writeMemory(getCellAtCoordinates(hoverTile.x, hoverTile.y), inputBuffer);
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
				if (pause_thread)
					break;
				// create hover rect
				std::string title = WINDOW_TITLE + " " + Utils::hexify(getCellAtCoordinates(event.motion.x, event.motion.y));
				SDL_SetWindowTitle(window, title.c_str());

				hoverTile.x = event.motion.x;
				while (hoverTile.x % rectWidth != 0)
					hoverTile.x--;
				hoverTile.y = event.motion.y;
				while (hoverTile.y % rectHeight != 0)
					hoverTile.y--;

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
	for (int y = 0; y < tilesPerLine; y++){

		for (int x = 0; x < tilesPerLine; x++){
			// Create a rect for each memory cell			
			SDL_Rect r;
			rectWidth	= r.w	= (WINDOW_WIDTH / tilesPerLine);
			rectHeight	= r.h	= (WINDOW_HEIGHT / tilesPerLine);
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
			switch (tilesPerLine){
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
				{
					SDL_Rect r1;
					r1.w = rectWidth - 1;
					r1.h = (rectHeight - 1) / 4;
					r1.y = r.y;		// height + 1 pixel spacing
					r1.x = r.x;		// width + 1 pixel spacing			
					SDL_RenderCopy(renderer, NULL, &r1, &r);

					SDL_Rect r2;
					r2.w = r1.w;
					r2.h = r1.h * 3;
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
					r2.h = r1.h*2;
					r2.y = r1.y + r1.h;	
					r2.x = r1.x;				
					SDL_RenderCopy(renderer, NULL, &r2, &r);


					SDL_Rect r3;
					r3.w = (rectWidth - 1) / 3;
					r3.h = r1.h;
					r3.y = r1.y + r1.h + r2.h;	
					r3.x = r.x;
					SDL_RenderCopy(renderer, NULL, &r3, &r);

					SDL_Rect r4;
					r4.w = (rectWidth - 1) / 3;
					r4.h = r1.h;
					r4.y = r1.y + r1.h + r2.h;
					r4.x = r.x + (((rectWidth - 1) / 3) * 2);
					SDL_RenderCopy(renderer, NULL, &r4, &r);

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


					std::string charRepresentation; 
					std::ostringstream oss;			
					oss << cellValue;
					charRepresentation = oss.str();

					SDL_Surface* surfaceMessage3 = TTF_RenderText_Solid(font, charRepresentation.c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_Texture* charValueText = SDL_CreateTextureFromSurface(renderer, surfaceMessage3); // convert it into a texture
					SDL_RenderCopy(renderer, charValueText, NULL, &r4);
					SDL_FreeSurface(surfaceMessage3);
					SDL_DestroyTexture(charValueText);

					SDL_Surface* surfaceMessage4 = TTF_RenderText_Solid(font, std::to_string(cellValue).c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
					SDL_Texture* decValueText = SDL_CreateTextureFromSurface(renderer, surfaceMessage4); // convert it into a texture
					SDL_RenderCopy(renderer, decValueText, NULL, &r3);
					SDL_FreeSurface(surfaceMessage4);
					SDL_DestroyTexture(decValueText);

					break;
				}

			}
		}
	}

	// Draw Hover Tile

	
	// Set background color
	SDL_SetRenderDrawColor(renderer, 255, 22, 22, 255);


	SDL_Rect rHover;
	rHover.w = rectWidth - 1;
	rHover.h = rectHeight - 1;
	rHover.x = hoverTile.x;
	rHover.y = hoverTile.y;
	SDL_RenderDrawRect(renderer, &rHover);


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

void MemoryGrid::exit(){
	thread_exit = 1;
	// Close and destroy the window
	SDL_DestroyWindow(window);
	// Clean up
	SDL_Quit();
}