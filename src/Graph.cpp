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


	for (int i = 0; i < length; i++){

		if (graph->voice.audioLength <= 0)
			stream[i] = graph->getSpec()->silence;      // 128 is silence in a uint8 stream
		else
		{
			stream[i] = graph->voice.getSample();
			graph->voice.audioPosition++;


			// Fill the graphBuffer with the first 1000 bytes of the wave for plotting
			if (graph->graphPointer < 999)
				graph->graphBuffer[graph->graphPointer++] = stream[i];

		}


	}
}


Graph::Graph()
{
	// spawn thread
	SDL_Thread *refresh_thread = SDL_CreateThread(graphThreadFunc, NULL, this);
}

SDL_AudioSpec* Graph::getSpec(){
	return &this->spec;
}


void Graph::init()
{
	// Init SDL & SDL_ttf
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

#ifdef TTF_ENABLED
	TTF_Init();
	font = TTF_OpenFont("sans.ttf", 48);	//this opens a font style and sets a size
#endif

	SDL_zero(desiredDeviceSpec);

	desiredDeviceSpec.freq = 44100;         // Sample Rate
	desiredDeviceSpec.format = AUDIO_U8;    // Unsigned 8-Bit Samples
	desiredDeviceSpec.channels = 1;         // Mono
	desiredDeviceSpec.samples = 2048;       // The size of the Audio Buffer (in number of samples, eg: 2048 * 1 Byte (AUDIO_U8)
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
		WINDOW_TITLE.c_str(),              // window title
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
		voice.waveForm = Graph::Voice::WaveForm::NOISE;
		voice.amp = 120;
		voice.frequency = 440;
		SDL_PauseAudioDevice(dev, 1);        // play
		graphPointer = 0;

		voice.audioLength = 44100;
		voice.audioPosition = 0;
		voice.maxWaveValue = 2;

		SDL_PauseAudioDevice(dev, 0);        // play
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
						voice.waveForm = Graph::Voice::WaveForm::NOISE;
						break;
					}
					case Voice::NOISE:
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
					voice.frequency -= 10;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
					voice.frequency += 10;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
					voice.amp += 2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
					voice.amp -= 2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_P){
					if(voice.waveForm == Voice::WaveForm::RECT){
						if(voice.pwn + 0.05 < 1.05){
							voice.pwn += 0.05;
						}
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_O){
					if(voice.waveForm == Voice::WaveForm::RECT){
						if(voice.pwn - 0.05 > -0.05){
							voice.pwn -= 0.05;
						}
					}
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

			//SDL_PauseAudioDevice(dev, 1);      // play
			graphPointer = 0;

			voice.audioLength = 44100;
			voice.audioPosition = 0;

			SDL_PauseAudioDevice(dev, 0);        // play
			SDL_Delay(200);

			drawGraph();
		}


		//voice.waveForm = Voice::WaveForm::TRIANGLE;       

		//SDL_Delay(n);                      // delay the program to prevent the voice to be overridden before it has been played to the end
		//SDL_PauseAudioDevice(dev, 1);      // pause   

		SDL_Delay(REFRESH_INTERVAL);
		//SDL_PauseAudioDevice(dev, 1);      // pause   
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
		//SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT - y);
		if (x > 0)
			SDL_RenderDrawLine(renderer, prevX, prevY, x, WINDOW_HEIGHT - y);
		prevX = x;
		prevY = WINDOW_HEIGHT - y;
	}


	/* Texts */
#ifdef TTF_ENABLED
	SDL_Color textColor = { 222, 22, 22 };
	
	int w = 0;

	std::string waveform;
	switch (voice.waveForm){
	case Voice::WaveForm::SINE:{
		waveform = "Sine";
		w = 48;
		break;
	}
	case Voice::WaveForm::RECT:{
		waveform = "Rect";
		w = 48;
		break;
	}
	case Voice::WaveForm::TRIANGLE:{
		waveform = "Triangle";
		w = 48*2;
		break;
	}
	case Voice::WaveForm::SAWTOOTH:{
		waveform = "Sawtooth";
		w = 48*2;
		break;
	}
	case Voice::WaveForm::NOISE:{
		waveform = "Noise";
		w = 48*2;
		break;
	}
	}


	std::string amp = "Amp = " + std::to_string(this->voice.amp);
	w += 48 * 3;
	std::string freq = "Freq = " + std::to_string(this->voice.frequency) + " Hz";
	w += 48 * 3;

	std::string result = waveform + "  (" + amp + "," + freq + ")";

	// Different Display for Rectangle Waveform
	if(voice.waveForm == Voice::WaveForm::RECT){
			std::string pwn = "PWN = " + std::to_string(this->voice.pwn);
			w += 48 * 3;
			result = waveform + "  (" + amp + "," + freq + "," + pwn + ")";
	}

	SDL_Rect posWaveForm = { 10, 10, w, 15 };
	SDL_Surface* surfaceMessageWaveForm = TTF_RenderText_Solid(font, result.c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Texture* waveFormText = SDL_CreateTextureFromSurface(renderer, surfaceMessageWaveForm); // convert it into a texture
	SDL_RenderCopy(renderer, waveFormText, NULL, &posWaveForm);
	SDL_FreeSurface(surfaceMessageWaveForm);
	SDL_DestroyTexture(waveFormText);







#endif

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


uint8_t Graph::Voice::getSample(){
	double stepsPerPeriod = 44100.0 / (double)frequency;
	int min;

	switch (waveForm){
	case SINE:
	{
		float sineStep = 2 * M_PI * audioPosition * frequency / 44100;
		return (amp * sin(sineStep)) + 128;
		break;
	}
	case RECT:
		if (fmod((double)audioPosition, stepsPerPeriod) < pwn * stepsPerPeriod)
			return (amp * 1) + 128;
		else
			return (amp * -1) + 128;
		break;
	case SAWTOOTH:
		return amp * (fmod((maxWaveValue / stepsPerPeriod * (double)audioPosition), maxWaveValue) - maxWaveValue / 2) + 128;
		break;

	case TRIANGLE:
		return amp * (fabs(fmod((2.0 * maxWaveValue / stepsPerPeriod) * (double)audioPosition, 2.0 * maxWaveValue) - maxWaveValue) - maxWaveValue / 2) + 128;
		break;
	case NOISE:
		// maxWaveValue 2 -> Random number between -1.0 and 1.0
		return amp * (fmod((double)rand(), (maxWaveValue + 1.0)) - maxWaveValue / 2) + 128;
		break;
	default:
		return 0;
	}
}
