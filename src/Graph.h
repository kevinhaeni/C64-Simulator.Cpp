#ifndef GRAPH_H
#define GRAPH_H
#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <cmath>
#include <string>
#include <stack>
#include <io.h>	// unistd.h for mac/linux, io.h for windows
#include <vector>
#include <fstream>

/* Window Constants */
const std::string WINDOW_TITLE = "Wave Graph";
const int REFRESH_INTERVAL = 50;                // mseconds
const int WINDOW_WIDTH = 1980;
const int WINDOW_HEIGHT = 255;

/* Audio Constants */
const int SAMPLING_RATE = 44100;				// number of samples per second

#define TTF_ENABLED 1							// disable TTF on systems without sdl_ttf extension
#define DEBUG 0

#ifdef TTF_ENABLED
#include "SDL_ttf.h"
#endif

class Graph
{
private:
	SDL_Window *window;

	// SDL audio stuff
	SDL_AudioSpec desiredDeviceSpec;
	SDL_AudioSpec spec;
	SDL_AudioDeviceID dev;

	int thread_exit = 0;
	bool pause_thread = false;

#ifdef TTF_ENABLED
	TTF_Font* font;
#endif

public:
	Graph();
	void init();
	void mainLoop();
	void drawGraph();

	bool keyGpressed;

	void exit();
	SDL_AudioSpec* getSpec();


	// SDL audio members
	struct Voice{
		Voice();

		bool silent = false;
		// WaveForm parameters
		enum WaveForm{
			SINE = 1, RECT = 2, SAWTOOTH = 3, TRIANGLE = 4, NOISE = 5
		} waveForm;
		int frequency;              // the frequency of the voice
		int amp;                    // the amplitude of the voice
		double pwn = 0.5;            // Square wave width, 0 - 1.0
		double maxWaveValue;  //
		long double phase;
		long double phaseInc;

		// SDL buffer handling members
		int audioPosition = 0;      // counter
		bool gate = false;

		uint8_t getSample();

		// ADSR Envelope extension
		struct Envelope
		{
			bool active = false;
			// 0-16 Value
			uint8_t attack_index;
			uint8_t decay_index;
			uint8_t sustain_index;
			uint8_t release_index;

			bool gate;
			bool holdZero;

			Envelope();
			void set_gate(bool setIt);
			void reset();
			// do a cycle step
			void doStep();

			// bug, sets 0 as default, float error
			uint16_t cyclesWhenToChangeEnvelopeCounter;

			uint8_t envelope_counter;
			double envelope_counter_forWave;
			double get_envelope_counter() const;

			int cycleCounter;

			enum State{
				ATTACK, DECAY_SUSTAIN, RELEASE
			} state;

			static const uint8_t sustain_level[16];

			static const uint8_t decreasePerEnvelopeValue[256];

			// Todo: should be fixed coded

			// number of cycles till the next increase of the enveloper_counter
			static const uint16_t cyclesWhenToChangeEnvelopeCounter_Attack[16];

			struct Instrument{
				std::string name;
				uint8_t attack_index;
				uint8_t decay_index;
				uint8_t sustain_index;
				uint8_t release_index;
				Instrument(std::string, uint8_t, uint8_t, uint8_t, uint8_t);
			};

			std::vector <Instrument> instruments;
			int active_instrument_index;
			void next_instrument();
			void set_instrument();

		} envelope;
	} voice;

	int graphDisplayLength = 9900;

	// Graph members
	int graphPointer = 0;
	uint8_t* graphBuffer;		// size must be a multiple of the window width (default 1980)
	int graphBufferSize = SAMPLING_RATE;
	void resetGraphBuffer();

	//int prevX = 0;
	//int prevY = 0;

	std::ofstream logFile;
	std::ifstream* envFile;

	int logCounter = 0;
};


#endif