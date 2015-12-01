#include "Graph.h"
#include <iostream>
#include <string>
#include <algorithm>    // std::min

double cotan(double i) { return(1 / tan(i)); }

const uint8_t Graph::Voice::Envelope::sustain_level[16] = {
	0x00,
	0x11,
	0x22,
	0x33,
	0x44,
	0x55,
	0x66,
	0x77,
	0x88,
	0x99,
	0xaa,
	0xbb,
	0xcc,
	0xdd,
	0xee,
	0xff,
};

const uint8_t Graph::Voice::Envelope::decreasePerEnvelopeValue[256] = {
	/* 0x00: */   1, 30, 30, 30, 30, 30, 30, 16, // 0x06
	/* 0x08: */  16, 16, 16, 16, 16, 16, 16, 8,  // 0x0e
	/* 0x10: */   8, 8, 8, 8, 8, 8, 8, 8,
	/* 0x18: */   8, 8, 8, 4, 4, 4, 4, 4,		 // 0x1a
	/* 0x20: */   4, 4, 4, 4, 4, 4, 4, 4,
	/* 0x28: */   4, 4, 4, 4, 4, 4, 4, 4,
	/* 0x30: */   4, 4, 4, 4, 4, 4, 4, 2,		 // 0x36
	/* 0x38: */   2, 2, 2, 2, 2, 2, 2, 2,
	/* 0x40: */   2, 2, 2, 2, 2, 2, 2, 2,
	/* 0x48: */   2, 2, 2, 2, 2, 2, 2, 2,
	/* 0x50: */   2, 2, 2, 2, 2, 2, 2, 2,
	/* 0x58: */   2, 2, 2, 2, 2, 2, 1, 1,		  // 0x5d
	/* 0x60: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x68: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x70: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x78: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x80: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x88: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x90: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0x98: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xa0: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xa8: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xb0: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xb8: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xc0: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xc8: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xd0: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xd8: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xe0: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xe8: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xf0: */   1, 1, 1, 1, 1, 1, 1, 1,
	/* 0xf8: */   1, 1, 1, 1, 1, 1, 1, 1
};

// number of cycles till the next increase of the enveloper_counter
const uint16_t Graph::Voice::Envelope::cyclesWhenToChangeEnvelopeCounter_Attack[16] = {
	1, //Bug < 0 //abs(2*SAMPLING_RATE/256/1000),		//   2ms*1.0MHz/256 =     7.81
	abs(8 * SAMPLING_RATE / 256 / 1000),				//   8ms*1.0MHz/256 =    31.25
	abs(16 * SAMPLING_RATE / 256 / 1000),				//  16ms*1.0MHz/256 =    62.50
	abs(24 * SAMPLING_RATE / 256 / 1000),				//  24ms*1.0MHz/256 =    93.75
	abs(38 * SAMPLING_RATE / 256 / 1000),				//  38ms*1.0MHz/256 =   148.44
	abs(56 * SAMPLING_RATE / 256 / 1000),				//  56ms*1.0MHz/256 =   218.75
	abs(68 * SAMPLING_RATE / 256 / 1000),				//  68ms*1.0MHz/256 =   265.63
	abs(80 * SAMPLING_RATE / 256 / 1000),				//  80ms*1.0MHz/256 =   312.50
	abs(100 * SAMPLING_RATE / 256 / 1000),				// 100ms*1.0MHz/256 =   390.63
	abs(250 * SAMPLING_RATE / 256 / 1000),				// 250ms*1.0MHz/256 =   976.56
	abs(500 * SAMPLING_RATE / 256 / 1000),				// 500ms*1.0MHz/256 =  1953.13
	abs(800 * SAMPLING_RATE / 256 / 1000),				// 800ms*1.0MHz/256 =  3125.00
	abs(1000 * SAMPLING_RATE / 256 / 1000),				//   1 s*1.0MHz/256 =  3906.25
	abs(3000 * SAMPLING_RATE / 256 / 1000),				//   3 s*1.0MHz/256 = 11718.75
	abs(5000 * SAMPLING_RATE / 256 / 1000),				//   5 s*1.0MHz/256 = 19531.25
	abs(8000 * SAMPLING_RATE / 256 / 1000)				//   8 s*1.0MHz/256 = 31250.00
};

/*int main(int argc, char* argv[]){

	Graph* g = new Graph();

	int i;
	std::cin >> i;
	return 0;
}

*/
int graphThreadFunc(void *pointer){
	Graph* grid = static_cast<Graph*>(pointer);
	grid->init();

	return 0;
}



// SDL calls this function whenever it wants its buffer to be filled with samples
void SDLAudioCallback(void *data, Uint8 *buffer, int length){
	uint8_t *stream = static_cast<uint8_t*>(buffer);

	Graph* graph = static_cast<Graph*>(data);


	for (int i = 0; i < length; i++){

		if (graph->voice.silent)
			stream[i] = graph->getSpec()->silence;      // silence in uint8 is represented by the value 128
		else
		{
			stream[i] = graph->voice.getSample();		// calculate the next sample value

			// graph plotting extension:
			// Fill the graphBuffer with the first 9900 bytes of the wave for plotting
			if (graph->graphPointer < graph->graphBufferSize)
				graph->graphBuffer[graph->graphPointer++] = stream[i];

#ifdef DEBUG
			// write buffer value to logfile for debugging purposes
			if (graph->logCounter++ <= SAMPLING_RATE)
				graph->logFile << (int)stream[i] << std::endl;
#endif

			graph->voice.audioPosition++;
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
#ifdef DEBUG
	logFile.open("wavegenerator.log");
#endif

	// Init SDL & SDL_ttf
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

#ifdef TTF_ENABLED
	TTF_Init();
	font = TTF_OpenFont("sans.ttf", 48);		//this opens a font style and sets a size
#endif

	SDL_zero(desiredDeviceSpec);

	// define the audioDeviceSpec we would like to get
	desiredDeviceSpec.freq = SAMPLING_RATE;		// Sample Rate
	desiredDeviceSpec.format = AUDIO_U8;		// Unsigned 8-Bit Samples
	desiredDeviceSpec.channels = 1;				// Mono
	desiredDeviceSpec.samples = 2048;			// The size of the Audio Buffer (in number of samples, eg: 2048 * 1 Byte (AUDIO_U8)
	desiredDeviceSpec.callback = SDLAudioCallback;
	desiredDeviceSpec.userdata = this;

	// request audioDeviceSpec
	dev = SDL_OpenAudioDevice(nullptr, 0, &desiredDeviceSpec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (dev == 0) {
		printf("\nFailed to open audio: %s\n", SDL_GetError());
	}
	else {
		SDL_PauseAudioDevice(dev, 1);	// pause audio device
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
	if (window == nullptr) {
		// In case the window could not be created...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}
	else{

		envFile = new std::ifstream("adsr.txt");

		graphBuffer = new uint8_t[graphBufferSize];
		//graphBufferSize = graphDisplayLength;
		graphPointer = 0;

		SDL_PauseAudioDevice(dev, 0);        // play
		SDL_Delay(1000);	// 44100 / length of the audio  * 1000 (to get milliseconds)

		drawGraph();

		mainLoop();
		return;
	}
}

void Graph::mainLoop()
{
	// poll SDL events until we terminate the thread
	while (thread_exit == 0){
		SDL_Event event;

		bool forceRedraw = false;		// set to true when the audio wave changes and its graph should be redrawn

		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
					forceRedraw = true;
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
					forceRedraw = true;
					voice.frequency -= 10;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
					forceRedraw = true;
					voice.frequency += 10;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
					forceRedraw = true;
					voice.amp += 2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
					forceRedraw = true;
					voice.amp -= 2;
				}

				else if (event.key.keysym.scancode == SDL_SCANCODE_G){
					// toggle gate ON
					//graphPointer = 0;
					if (keyGpressed == false){
						voice.silent = false;
					
						graphPointer = 0;
						voice.audioPosition = 0;
						voice.phase = 0;
						voice.phaseInc = static_cast<double>(voice.frequency) / static_cast<double>(SAMPLING_RATE);
						voice.envelope.set_gate(true);

						SDL_PauseAudioDevice(dev, 0);        // play
					}
					keyGpressed = true;
				}

				else if (event.key.keysym.scancode == SDL_SCANCODE_E){
					forceRedraw = true;
					voice.envelope.active = !voice.envelope.active;
					voice.silent = true;
				}

				else if (event.key.keysym.scancode == SDL_SCANCODE_I){
					forceRedraw = true;
					if (++voice.envelope.active_instrument_index >= voice.envelope.instruments.size()){
						voice.envelope.active_instrument_index = 0;
					}
					voice.envelope.set_instrument();
				}

				else if (event.key.keysym.scancode == SDL_SCANCODE_P){
					forceRedraw = true;
					if (voice.waveForm == Voice::WaveForm::RECT){
						if (voice.pwn + 0.05 < 1.05){
							voice.pwn += 0.05;
						}
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_R){					
					if (voice.envelope.active){
						// read values from file
						int waveform, a, f, pwn, attack, decay, sustain, release, gateOpenDuration;

						if (*envFile >> waveform >> a >> f >> pwn >> attack >> decay >> sustain >> release >> gateOpenDuration)
						{
							voice.waveForm = static_cast<Voice::WaveForm>(waveform);
							voice.amp = a;
							voice.frequency = f;
							voice.pwn = pwn;
							voice.phase = 0;
							voice.phaseInc = static_cast<double>(voice.frequency) / static_cast<double>(SAMPLING_RATE);

							voice.envelope.attack_index = attack;
							voice.envelope.decay_index = decay;
							voice.envelope.sustain_index = sustain;
							voice.envelope.release_index = release;
														
							voice.envelope.set_gate(true);
							graphPointer = 0;
							voice.audioPosition = 0;							
							SDL_PauseAudioDevice(dev, 0);        // play

							// wait to close gate
							SDL_Delay(gateOpenDuration);
							voice.envelope.set_gate(false);

							SDL_Delay(500);	// wait to allow the graph buffer to be filled

							drawGraph();

						} else
						{
							// reopen file
						}					

					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_O){
					forceRedraw = true;
					if (voice.waveForm == Voice::WaveForm::RECT){
						if (voice.pwn - 0.05 > -0.05){
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
			}
			case SDL_KEYUP:
			{
				//				hasChanged = true;

				if (event.key.keysym.scancode == SDL_SCANCODE_G){
					//					hasChanged = true;
					// toggle gate OFF
					voice.envelope.set_gate(false);
					keyGpressed = false;

					SDL_Delay(1000);

					drawGraph();
				}
			}
			default: /* unhandled event */
				break;
			}
		}

		if (!pause_thread && forceRedraw)
		{

			//SDL_PauseAudioDevice(dev, 1);      // play
			graphPointer = 0;

			voice.audioPosition = 0;
			voice.phaseInc = static_cast<double>(voice.frequency) / static_cast<double>(SAMPLING_RATE);

			SDL_PauseAudioDevice(dev, 0);        // play
			SDL_Delay(1000);

			drawGraph();
		}

		SDL_Delay(REFRESH_INTERVAL);
	}
	return;
}

void Graph::drawGraph()
{
	SDL_Renderer *renderer = SDL_GetRenderer(window);
	if (renderer == nullptr)
		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	// Set colors and clear windows
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);

	int x = 0;	// x-pixel loop
	int i = 0;	// graphArray index

	int condenseFactor = graphDisplayLength / WINDOW_WIDTH;		// example: if we want to show 10000 samples on a 2000 pixel screen, we need to condense 5 pixels together
	if (condenseFactor <= 2)
		condenseFactor = 2;
	while (x < WINDOW_WIDTH){
		uint8_t* condensingValues = new uint8_t[condenseFactor];
		for (int j = 0; j < condenseFactor; j++)
		{
			uint8_t value = graphBuffer[i + j];
			condensingValues[j] = value;
		}

		int x1 = x;
		int x2 = x;


		x++;
		i += condenseFactor - 1;


		uint8_t min = condensingValues[0];
		uint8_t max = condensingValues[0];
		for (int j = 0; j < condenseFactor; j++)
		{
			if (condensingValues[j] <= min)
				min = condensingValues[j];
			if (condensingValues[j] >= max)
				max = condensingValues[j];
		}

		bool rising;
		if (static_cast<int>(condensingValues[0]) <= static_cast<int>(condensingValues[condenseFactor - 1]))
			rising = true;
		else
			rising = false;

		int y1 = WINDOW_HEIGHT - min;
		int y2 = WINDOW_HEIGHT - max;

		if (rising){
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		}
		else{
			SDL_RenderDrawLine(renderer, x1, y2, x2, y1);
		}

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
		w = 48 * 2;
		break;
	}
	case Voice::WaveForm::SAWTOOTH:{
		waveform = "Sawtooth";
		w = 48 * 2;
		break;
	}
	case Voice::WaveForm::NOISE:{
		waveform = "Noise";
		w = 48 * 2;
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
	if (voice.waveForm == Voice::WaveForm::RECT){
		std::string pwn = "PWN = " + std::to_string(this->voice.pwn);
		w += 48 * 3;
		result = result + ", " + pwn;
	}
	// Different Display for Envelope
	if (voice.envelope.active == true){
		std::string gate = "Gate = " + std::to_string(this->voice.envelope.gate);

		std::string inst = "Instrument = " + this->voice.envelope.instruments[this->voice.envelope.active_instrument_index].name;
		w += 48 * 3;
		result = result + ", " + gate + ", " + inst;
	}

	SDL_Rect posWaveForm = { 10, 10, w, 15 };
	SDL_Surface* surfaceMessageWaveForm = TTF_RenderText_Solid(font, result.c_str(), textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Texture* waveFormText = SDL_CreateTextureFromSurface(renderer, surfaceMessageWaveForm); // convert it into a texture
	SDL_RenderCopy(renderer, waveFormText, nullptr, &posWaveForm);
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
	// Initial wave parameters
	waveForm = Graph::Voice::WaveForm::SINE;
	amp = 120;
	frequency = 440;
	audioPosition = 0;
	maxWaveValue = 2;
	phase = 0;
	phaseInc = static_cast<double>(frequency) / static_cast<double>(SAMPLING_RATE);

	envelope.reset();
	envelope.set_instrument();
}

Graph::Voice::Envelope::Envelope(){
	// Xylophone, Triangle
	Instrument * i1 = new Instrument("Piano (Pulse)", 0, 9, 0, 0);
	// Trumpet, Sawtooth
	Instrument * i2 = new Instrument("Trumpet (Sawtooth)", 6, 0, 4, 0);
	// Accordion, Triangle
	Instrument * i3 = new Instrument("Flute (Triangle)", 6, 0, 4, 0);

	instruments.push_back(*i1);
	instruments.push_back(*i2);
	instruments.push_back(*i3);
}


uint8_t Graph::Voice::getSample(){
	//uint16_t stepsPerPeriod = SAMPLING_RATE / frequency;
	uint16_t stepsPerPeriod = 1 / phaseInc;
	//uint16_t stepCounter = audioPosition % SAMPLING_RATE;
	int stepCounter = audioPosition % SAMPLING_RATE;

	double env;

	if (envelope.active == true){
		// check if we are at the beginning of a new wave period -> 0/440Hz
		// we can only adjust the envelope value at the beginning of a Wave
		// 440hz -> we change the envelope 440times per second

		//if (stepCounter % stepsPerPeriod == 0){
		float rounded_up_phase = ceilf(fmod(phase,1) * 100) / 100;
		if (fmod(rounded_up_phase, 1) >= 0.99){
			envelope.envelope_counter_forWave = envelope.get_envelope_counter();
		}
		// we calculate a new envelope value if we reach the "cycles when to change the envelope counter" defined in the table
		if (stepCounter % envelope.cyclesWhenToChangeEnvelopeCounter == 0){
			envelope.doStep();
		}
		env = envelope.envelope_counter_forWave;

	}
	else
	{
		env = 1;
	}


	switch (waveForm){
	case SINE:
	{
		/*float sineStep = 2 * M_PI * audioPosition * frequency / SAMPLING_RATE;
		return (amp * env * sin(sineStep)) + 128;*/
		uint8_t sample = (amp * env * sin(2 * M_PI * phase)) + 128;
		phase += phaseInc;
		return sample;
	}
	case RECT:
	{
		//if (fmod(static_cast<double>(audioPosition), stepsPerPeriod) < pwn * stepsPerPeriod)
		uint8_t sample;
		if (fmod(phase, stepsPerPeriod*phaseInc) < pwn * stepsPerPeriod*phaseInc)
		{
			phase += phaseInc;
			sample = (amp * env * 1) + 128;
		}
		else
		{
			phase += phaseInc;
			sample = (amp * env * -1) + 128;
		}
		return sample;
	}
	case SAWTOOTH:
	{		
		//return  amp * env * (fmod((maxWaveValue / stepsPerPeriod * static_cast<double>(audioPosition)), maxWaveValue) - maxWaveValue / 2) + 128;
		uint8_t sample;
		sample = env* -(2 * amp) / M_PI * atan(cotan(phase*M_PI)) + 128;
		phase += phaseInc;
		return sample;
	}
	case TRIANGLE:
	{
		// sine-to-triangle equation from wikipedia (https://en.wikipedia.org/wiki/Triangle_wave)
		uint8_t sample;
		double sinevalue = (sin(2 * M_PI * phase));

		sample = env * (2 * amp) / M_PI * asin(sinevalue) + 128;

		phase += phaseInc;
		return sample;

	}

	case NOISE:
		// maxWaveValue 2 -> Random number between -1.0 and 1.0
		return amp * env * (fmod(static_cast<double>(rand()), (maxWaveValue + 1.0)) - maxWaveValue / 2) + 128;
	default:
		return 0;
	}
}


void Graph::Voice::Envelope::doStep(){

	// counter stays at zero
	if (holdZero == true){
		return;
	}
	switch (state){
	case(ATTACK) :
		// we do a increase of the envelope_counter only, if we reached the amount of cycles
		// when we have to do a envelope step
		++envelope_counter &= 0xff;
		if (envelope_counter == 0xff){
			// we reached the peak
			state = State::DECAY_SUSTAIN;
			cyclesWhenToChangeEnvelopeCounter = 3 * cyclesWhenToChangeEnvelopeCounter_Attack[decay_index];
		}
		break;
	case(DECAY_SUSTAIN) :
		if (envelope_counter != sustain_level[sustain_index]){
			envelope_counter--;
		}
						break;
	case(RELEASE) :
		envelope_counter--;
		break;
	}
	if (envelope_counter == 0x00){
		holdZero = true;
	}

}

double Graph::Voice::Envelope::get_envelope_counter() const
{
	return static_cast<double>(envelope_counter) / 255.0;
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
	if (!gate && setIt){
		state = State::ATTACK;
		cyclesWhenToChangeEnvelopeCounter = cyclesWhenToChangeEnvelopeCounter_Attack[attack_index];
		holdZero = false;
	}
	// Gate now disabled
	else if (gate && !setIt){
		state = State::RELEASE;
		cyclesWhenToChangeEnvelopeCounter = 3 * cyclesWhenToChangeEnvelopeCounter_Attack[release_index];
	}
	gate = setIt;
}

Graph::Voice::Envelope::Instrument::Instrument(std::string name, uint8_t a, uint8_t d, uint8_t s, uint8_t r){
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


void Graph::resetGraphBuffer(){
	for (int i = 0; i < graphBufferSize; i++){
		graphBuffer[i] = getSpec()->silence;
	}
}
