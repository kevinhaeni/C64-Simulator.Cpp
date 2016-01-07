#include "MemoryGrid.h"

// Reads the value at the given memory address
// also converts the [9] bitwise representation (e.g.: "01011101\0") into a unsigned decimal number
uint8_t MemoryGrid::readMemory(uint16_t addr) const
{
	uint8_t val = 0;
	for (int i = 7; i >= 0; i--){
		if ((*_mem)[addr][i] == '1'){
			val += pow(2, 7 - i);
		}
	}
	return val;
}

// Writes the value to the given memory address
void MemoryGrid::writeMemory(uint8_t value, uint16_t addr) const{

	//std::cout <<  "Write Memory" << std::endl << "Before: " << readMemory(addr) << std::endl;
	for (int i = 7; i >= 0; i--){
		if (value & 0x01){
			(*_mem)[addr][i] = '1';
		}
		else{
			(*_mem)[addr][i] = '0';
		}
		value >>= 1;
	}
	//std::cout <<  "After: " << readMemory(addr) << std::endl;
}

int threadFunc(void *pointer){
	MemoryGrid* grid = (MemoryGrid*)pointer;
	grid->init();

	return 0;
}

void MemoryGrid::dispatchEvent(void* ev)
{

	SDL_Event* event = (SDL_Event*)ev;
	switch (event->type)
	{
	case SDL_KEYDOWN:
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE){
			pause_thread = !pause_thread;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
			exit();
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_RETURN){
			if (pause_thread){
				// submit
				// convert hex string to hex int (eg. convert "ff" to 255)
				std::string inputBufferStr(inputBuffer);
				unsigned int hexIntVal = std::stoul(inputBufferStr, nullptr, 16);

				// store the new value to the memory
				writeMemory(hexIntVal, getCellAtCoordinates(hoverTile.x, hoverTile.y));
			}
		}
		else{
			if (pause_thread){
				if (inputBufferPos < 2)
				{
					// If it's the first key
					inputBuffer[inputBufferPos++] = event->key.keysym.sym;
				}else{
					// if it's the third, fourth, etc. key, shift the whole string by one position to the right (replace the char at index 0)
					inputBuffer[0] = inputBuffer[1];
					inputBuffer[1] = event->key.keysym.sym;
					inputBufferPos++;
				}
				

			}
		}

		break;
	}
	case SDL_MOUSEMOTION:
	{
		// change the window title and the color the memory cell red where the mouse pointer is pointing to

		if (pause_thread)
			break;
		// create hover rect
		std::string title = WINDOW_TITLE + " " + Utils::hexify(getCellAtCoordinates(event->motion.x, event->motion.y));
		SDL_SetWindowTitle(window, title.c_str());

		hoverTile.x = event->motion.x;
		while (hoverTile.x % rectWidth != 0)
			hoverTile.x--;
		hoverTile.y = event->motion.y;
		while (hoverTile.y % rectHeight != 0)
			hoverTile.y--;

		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		// Zoom in if the left mouse button is clicked, zoom out if the right mouse button is clicked
		if (event->button.x == 0 || event->button.y == 0)
			return;

		if (event->button.button == 1){
			// left click (zoom in))
			int xcoord = event->button.x;
			int ycoord = event->button.y;
			handleZoom(xcoord, ycoord, -1);
		}
		else if (event->button.button == 3){
			// right click (zoom out)
			int xcoord = event->button.x;
			int ycoord = event->button.y;
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

MemoryGrid::MemoryGrid(memory* mem)
	: tilesPerLine(256)
{
	_mem = mem;
	
	window = SDL_CreateWindow(
		WINDOW_TITLE.c_str(),  			   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		SDL_WINDOW_SHOWN                   // flags - see below
		);

	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	

	// spawn thread
	SDL_Thread *refresh_thread = SDL_CreateThread(threadFunc, NULL, this);
	//this->init();
}


MemoryGrid::~MemoryGrid(){

}


void MemoryGrid::init()
{
	
/*	OpenGL doesn't seem to be supported on our linux machines without installing new drivers -> f*** it

	// Settings
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
*/

	font = TTF_OpenFont("sans.ttf", 158);	// sets the font / size

	// Create an application window with the following settings:
	

	// Check if the window was successfully created
	if (window == NULL) {
		// In case the window could not be created...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}
	else{
		printf("memorygrid init finished..entering main loop");
		SDL_Delay(10);
		//drawGrid();
		//mainLoop();
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


void MemoryGrid::drawGrid()
{
	
	if (renderer == nullptr)
		return;

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
			uint8_t cellValue = readMemory(cellAddress);

			// Set colors (Blue = has data, white = no data, red = current PC)		
			//if (cellAddress == getCPU()->Registers.PC){
			// HK: TODO: Current program counter!
			if (cellAddress == 0x0000){
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
	
//	SDL_Delay(REFRESH_INTERVAL);

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
