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
		voice.waveForm = Graph::Voice::WaveForm::SAWTOOTH;
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

				else if (event.key.keysym.scancode == SDL_SCANCODE_G){
					// toggle gate ON
					if(keyGpressed == false){
						voice.envelope.set_gate(true);
					}
					keyGpressed = true;
				}

				else if (event.key.keysym.scancode == SDL_SCANCODE_E){
					if(voice.envelope.active == true){
						voice.envelope.active = false;
					} else{
						voice.envelope.active = true;
					}
				}

				else if (event.key.keysym.scancode == SDL_SCANCODE_I){
					if(++voice.envelope.active_instrument_index >= voice.envelope.instruments.size()){
						voice.envelope.active_instrument_index = 0;
					}
					voice.envelope.set_instrument();
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
			case SDL_KEYUP:
			{
				hasChanged = true;

				if (event.key.keysym.scancode == SDL_SCANCODE_G){
					// toggle gate OFF
					voice.envelope.set_gate(false);
					keyGpressed = false;
				}
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
	std::string env = "Envelope = " + std::to_string(this->voice.envelope.active);
	w += 48 * 3;

	std::string result = waveform + " : " + amp + ", " + freq + ", " + env;

	// Different Display for Rectangle Waveform
	if(voice.waveForm == Voice::WaveForm::RECT){
		std::string pwn = "PWN = " + std::to_string(this->voice.pwn);
		w += 48 * 3;
		result = result + ", " + pwn;
	}
	// Different Display for Envelope
	if(voice.envelope.active == true){
		std::string gate = "Gate = " + std::to_string(this->voice.envelope.gate);

		int i = this->voice.envelope.active_instrument_index;
		std::string inst = "Instrument = " + this->voice.envelope.instruments[i].name;
		w += 48 * 3;
		result = result + ", " + gate + ", " + inst;
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

Graph::Voice::Voice(){
	envelope.reset();
	envelope.set_instrument();
}

Graph::Voice::Envelope::Envelope(){
	// Xylophone, Triangle
	Instrument * i1 = new Instrument("Xylophone (Triangle)",0,9,0,0);
	// Trumpet, Sawtooth
	Instrument * i2 = new Instrument("Trumpet (Sawtooth)",6,0,0,0);
	// Accordion, Triangle
	Instrument * i3 = new Instrument("Accordion (Triangle)",6,6,0,0);

	instruments.push_back(*i1);
	instruments.push_back(*i2);
	instruments.push_back(*i3);
}


uint8_t Graph::Voice::getSample(){
	uint16_t stepsPerPeriod = 44100 / frequency;
	uint16_t stepCounter = audioPosition % 44100;

	int min;
	double env;


	if(envelope.active == true){
		// check if we are at the beginning of a new wave period -> 0/440Hz
		// we can only adjust the envelope value at the beginning of a Wave
		// 440hz -> we change the envelope 440times per second
		if(stepCounter % stepsPerPeriod == 0){
			envelope.envelope_counter_forWave = envelope.get_envelope_counter();
		}
		// we calculate a new envelope value if we reach the "cycles when to change the envelope counter" defined in the table
		if(stepCounter % envelope.cyclesWhenToChangeEnvelopeCounter == 0){
			envelope.doStep();
		}
		env = envelope.envelope_counter_forWave;

	}else
	{
		env = 1;
	}


	switch (waveForm){
	case SINE:
	{
		float sineStep = 2 * M_PI * audioPosition * frequency / 44100;
		return (amp * env * sin(sineStep)) + 128;
		break;
	}
	case RECT:
		if (fmod((double)audioPosition, stepsPerPeriod) < pwn * stepsPerPeriod)
			return (amp * env *1) + 128;
		else
			return (amp * env * -1) + 128;
		break;
	case SAWTOOTH:
		return  amp * env * (fmod((maxWaveValue / stepsPerPeriod * (double)audioPosition), maxWaveValue) - maxWaveValue / 2) + 128;
		break;
	case TRIANGLE:
		return amp * env *(fabs(fmod((2.0 * maxWaveValue / stepsPerPeriod) * (double)audioPosition, 2.0 * maxWaveValue) - maxWaveValue) - maxWaveValue / 2) + 128;
		break;
	case NOISE:
		// maxWaveValue 2 -> Random number between -1.0 and 1.0
		return amp * env * (fmod((double)rand(), (maxWaveValue + 1.0)) - maxWaveValue / 2) + 128;
		break;
	default:
		return 0;
	}
}


void Graph::Voice::Envelope::doStep(){

	// counter stays at zero
	if(holdZero == true){
		return;
	}
	switch(state){
		case(ATTACK):
			// we do a increase of the envelope_counter only, if we reached the amount of cycles
			// when we have to do a envelope step
			++envelope_counter &= 0xff;
			if(envelope_counter == 0xff){
				// we reached the peak
				state = State::DECAY_SUSTAIN;
				cyclesWhenToChangeEnvelopeCounter = 3 * cyclesWhenToChangeEnvelopeCounter_Attack[decay_index];
			}
			break;
		case(DECAY_SUSTAIN):
			if(envelope_counter != sustain_level[sustain_index]){
				envelope_counter--;
			}
			break;
		case(RELEASE):
			envelope_counter--;
			break;
	}
	if(envelope_counter == 0x00){
		holdZero = true;
	}

}

double Graph::Voice::Envelope::get_envelope_counter(){
	return (double)envelope_counter / 255.0;
}

void Graph::Voice::Envelope::reset(){
	envelope_counter = 0;

	attack_index = 0;
	decay_index = 0;
	sustain_index = 0;
	release_index = 0;

	active_instrument_index = 0;

	gate = false;

	state = State::RELEASE;
	cyclesWhenToChangeEnvelopeCounter = 3 * cyclesWhenToChangeEnvelopeCounter_Attack[release_index];

	holdZero = true;
}

void Graph::Voice::Envelope::set_gate(bool setIt){

	// Gate now active
	if(!gate && setIt){
		state = State::ATTACK;
		cyclesWhenToChangeEnvelopeCounter = cyclesWhenToChangeEnvelopeCounter_Attack[attack_index];
		holdZero = false;
	}
	// Gate now disabled
	else if(gate && !setIt){
		state = State::RELEASE;
		cyclesWhenToChangeEnvelopeCounter = 3 * cyclesWhenToChangeEnvelopeCounter_Attack[release_index];
	}
	gate = setIt;
}

Graph::Voice::Envelope::Instrument::Instrument(std::string name, uint8_t a,uint8_t d,uint8_t s,uint8_t r){
	this->name = name;
	this->attack_index = a;
	this->decay_index = d;
	this->sustain_index = s;
	this->release_index = r;
}

void Graph::Voice::Envelope::set_instrument(){
	attack_index = instruments[active_instrument_index].attack_index;
	decay_index = instruments[active_instrument_index].decay_index;
	sustain_index = instruments[active_instrument_index].sustain_index;
	release_index = instruments[active_instrument_index].release_index;
}
