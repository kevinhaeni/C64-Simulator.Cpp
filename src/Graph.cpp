#include "Graph.h"
#include <thread>
#include <iostream>
#include <sstream>
#include <string>


int main(int argc, char* argv[]){

	Graph* g = new Graph();

	int i;
	std::cin >> i;

	return 0;
}

int graphThreadFunc(void *pointer){
	Graph* grid = (Graph*)pointer;
	grid->init();

	return 0;
}



// SDL calls this function whenever it wants its buffer to be filled with samples
void SDLAudioCallback(void *data, Uint8 *buffer, int length){
	uint8_t *stream = (uint8_t*)buffer;

	Graph* graph = (Graph*)data;


	for (int i = 0; i <= length; i++){


		if (graph->voice.audioLength <= 0)
			stream[i] = 0;
		else
		{
			stream[i] = graph->voice.getSample();
			graph->voice.audioPosition++;


			// Graph
			if (graph->graphPointer < 999)
				graph->graphBuffer[graph->graphPointer++] = stream[i];

		}


	}
}


Graph::Graph()
{
	//memcpy(buffer, buffer, 44100);
	// spawn thread
	SDL_Thread *refresh_thread = SDL_CreateThread(graphThreadFunc, NULL, this);
	//init();

}



void Graph::init()
{
	// Init SDL & SDL_ttf
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	SDL_zero(desiredDeviceSpec);

	desiredDeviceSpec.freq = 44100;			// Sample Rate
	desiredDeviceSpec.format = AUDIO_U8;	// Unsigned 8-Bit Samples
	desiredDeviceSpec.channels = 1;			// Mono
	desiredDeviceSpec.samples = 2048;		// The size of the Audio Buffer (in number of samples, eg: 2048 * 1 Byte (AUDIO_U8)
	desiredDeviceSpec.callback = SDLAudioCallback;
	desiredDeviceSpec.userdata = this;


	dev = SDL_OpenAudioDevice(NULL, 0, &desiredDeviceSpec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (dev == 0) {
		printf("\nFailed to open audio: %s\n", SDL_GetError());
	}
	else {
		SDL_PauseAudioDevice(dev, 1); /* pause! */
		SDL_PauseAudio(1);
	}

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
		voice.waveForm = Graph::Voice::WaveForm::SINE;
		voice.amp = 1;
		voice.frequency = 440;
		SDL_PauseAudioDevice(dev, 1);		 // play
		graphPointer = 0;

		voice.playForNMicroSeconds(2000);
		SDL_PauseAudioDevice(dev, 0);		 // play
		SDL_Delay(200);

		drawGraph();


		mainLoop();
		return;
	}
}

void Graph::mainLoop()
{
	while (thread_exit == 0){
		SDL_Event event;
		bool hasChanged = false;

		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
			{
				hasChanged = true;

				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
					//pause_thread = !pause_thread;
					switch (voice.waveForm){
					case Voice::SINE:
					{
						voice.waveForm = Graph::Voice::WaveForm::TRIANGLE;
						break;
					}
					case Voice::TRIANGLE:
					{
						voice.waveForm = Graph::Voice::WaveForm::RECT;
						break;
					}
					case Voice::RECT:
					{
						voice.waveForm = Graph::Voice::WaveForm::SAWTOOTH;
						break;
					}
					case Voice::SAWTOOTH:
					{
						voice.waveForm = Graph::Voice::WaveForm::SINE;
						break;
					}
					default:
						break;
					}				
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
					exit();
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN){
					
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
					voice.frequency -= 2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
					voice.frequency += 2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
					voice.amp += 2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
					voice.amp -= 2;
				}
				else{
					
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

		if (!pause_thread && hasChanged)
		{
			
			//SDL_PauseAudioDevice(dev, 1);		 // play
			graphPointer = 0;

			voice.playForNMicroSeconds(2000);
			SDL_PauseAudioDevice(dev, 0);		 // play
			SDL_Delay(200);

			drawGraph();
		}	


		//voice.waveForm = Voice::WaveForm::TRIANGLE;		
		
		//SDL_Delay(n);						 // delay the program to prevent the voice to be overridden before it has been played to the end
		//SDL_PauseAudioDevice(dev, 1);		 // pause	

		SDL_Delay(REFRESH_INTERVAL);
		//SDL_PauseAudioDevice(dev, 1);		 // pause	
	}


	return;
}

void Graph::drawGraph()
{

	SDL_Renderer *renderer = SDL_GetRenderer(window);
	if (renderer == nullptr)
		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	// Set background color
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Clear winow
	SDL_RenderClear(renderer);


	SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
	for (int x = 0; x < WINDOW_WIDTH; x++){
		uint8_t y = graphBuffer[x];
		SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT - y);
	}
	

	SDL_RenderPresent(renderer);


	return;
}

void Graph::exit(){
	thread_exit = 1;
	// Close and destroy the window
	SDL_DestroyWindow(window);
	// Clean up
	SDL_Quit();
}

void Graph::Voice::playForNMicroSeconds(int n){		 // calculates the number of samples and delays the program
	//samplesLeft = 44100 * n / 1000;
	audioLength = 44100;
	// Waveform length = Sampling rate / frequency
	audioLength = 44100 / frequency;

	audioPosition = 0;					 // reset the counter
	//SDL_PauseAudioDevice(dev, 0);		 // play
	//SDL_Delay(n);						 // delay the program to prevent the voice to be overridden before it has been played to the end
	//SDL_PauseAudioDevice(dev, 1);		 // pause		

}

uint8_t Graph::Voice::getSample(){

	int time = (audioPosition * frequency) / 44100;
	double step = 44100.0 / (double)frequency;

	uint8_t rect_value = 0x00;

	//std::cout << step << " - ";
	switch (waveForm){
	case SINE:
	{
		float sineStep = 2 * M_PI * audioPosition * frequency / 44100;
		return (amp * sin(sineStep)) + 127;		// +127 because otherwise the negative values will be cut off
		break;
	}
	case RECT:
		if (fmod((double)audioPosition, step) >= 0.5 * step){
			rect_value = 0xFF;
		}
		return amp * rect_value;
		break;

	case SAWTOOTH:
		return amp * fmod((double)audioPosition, step);
		break;

	case TRIANGLE:
		return amp * abs(fmod((double)audioPosition, step) - 0.5 * step);
		break;

	default:
		return 0;
	}
}