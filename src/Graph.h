#ifndef GRAPH_H
#define GRAPH_H
#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <cmath>
#include <string>
#include <stack>
#include <unistd.h>
#include <vector>

/* Constants */
const int REFRESH_INTERVAL = 50;                // mseconds
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 255;
const std::string WINDOW_TITLE = "Wave Graph";

#define TTF_ENABLED 1

#ifdef TTF_ENABLED
#include "SDL_ttf.h"
#endif

class Graph
{
private:
	SDL_Window *window;          // Declare a pointer

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

	struct Voice{
		Voice();

		int frequency;              // the frequency of the voice
		int amp;                    // the amplitude of the voice

		double pwn = 0.7;            // Square wave width, 0 - 1.0
		double maxWaveValue;  //


		int audioLength;            // number of samples to be played, eg: 1.2 seconds * 44100 samples per second
		int audioPosition = 0;      // counter

		enum WaveForm{
			SINE = 0, RECT = 1, SAWTOOTH = 2, TRIANGLE = 3, NOISE = 4
		} waveForm;

		uint8_t getSample();

		bool gate = false;



		struct Envelope
			{
				bool active = false;

				// SDL Frequency
				int systemFrequency = 44100;

				Envelope();


				// 0-16 Value
				uint8_t attack_index;
				uint8_t decay_index;
				uint8_t sustain_index;
				uint8_t release_index;

				bool gate;
				bool holdZero;

				void set_gate(bool setIt);
				void reset();
				// do a cycle step
				void doStep();

				// bug, sets 0 as default, float error
				uint16_t cyclesWhenToChangeEnvelopeCounter;

				uint8_t envelope_counter;
				double envelope_counter_forWave;
				double get_envelope_counter();

				enum State{
					ATTACK,DECAY_SUSTAIN,RELEASE
				} state;


				uint8_t sustain_level[16] = {
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

				uint8_t decreasePerEnvelopeValue[256] = {
				  /* 0x00: */   1, 30, 30, 30, 30, 30, 30, 16,  // 0x06
				  /* 0x08: */  16, 16, 16, 16, 16, 16, 16,  8,  // 0x0e
				  /* 0x10: */   8,  8,  8,  8,  8,  8,  8,  8,
				  /* 0x18: */   8,  8,  8,  4,  4,  4,  4,  4,  // 0x1a
				  /* 0x20: */   4,  4,  4,  4,  4,  4,  4,  4,
				  /* 0x28: */   4,  4,  4,  4,  4,  4,  4,  4,
				  /* 0x30: */   4,  4,  4,  4,  4,  4,  4,  2,  // 0x36
				  /* 0x38: */   2,  2,  2,  2,  2,  2,  2,  2,
				  /* 0x40: */   2,  2,  2,  2,  2,  2,  2,  2,
				  /* 0x48: */   2,  2,  2,  2,  2,  2,  2,  2,
				  /* 0x50: */   2,  2,  2,  2,  2,  2,  2,  2,
				  /* 0x58: */   2,  2,  2,  2,  2,  2,  1,  1,  // 0x5d
				  /* 0x60: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x68: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x70: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x78: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x80: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x88: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x90: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0x98: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xa0: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xa8: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xb0: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xb8: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xc0: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xc8: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xd0: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xd8: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xe0: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xe8: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xf0: */   1,  1,  1,  1,  1,  1,  1,  1,
				  /* 0xf8: */   1,  1,  1,  1,  1,  1,  1,  1
				};

				// Todo: should be fixed coded

				// number of cycles till the next increase of the enveloper_counter
				uint16_t cyclesWhenToChangeEnvelopeCounter_Attack[16] = {
					  1, //Bug < 0 //abs(2*systemFrequency/256/1000),  //   2ms*1.0MHz/256 =     7.81
					  abs(8*systemFrequency/256/1000),  //   8ms*1.0MHz/256 =    31.25
					  abs(16*systemFrequency/256/1000),  //  16ms*1.0MHz/256 =    62.50
					  abs(24*systemFrequency/256/1000),  //  24ms*1.0MHz/256 =    93.75
					  abs(38*systemFrequency/256/1000),  //  38ms*1.0MHz/256 =   148.44
					  abs(56*systemFrequency/256/1000),  //  56ms*1.0MHz/256 =   218.75
					  abs(68*systemFrequency/256/1000),  //  68ms*1.0MHz/256 =   265.63
					  abs(80*systemFrequency/256/1000),  //  80ms*1.0MHz/256 =   312.50
					  abs(100*systemFrequency/256/1000),  // 100ms*1.0MHz/256 =   390.63
					  abs(250*systemFrequency/256/1000),  // 250ms*1.0MHz/256 =   976.56
					  abs(500*systemFrequency/256/1000),  // 500ms*1.0MHz/256 =  1953.13
					  abs(800*systemFrequency/256/1000),  // 800ms*1.0MHz/256 =  3125.00
					  abs(1000*systemFrequency/256/1000),  //   1 s*1.0MHz/256 =  3906.25
					  abs(3000*systemFrequency/256/1000),  //   3 s*1.0MHz/256 = 11718.75
					  abs(5000*systemFrequency/256/1000),  //   5 s*1.0MHz/256 = 19531.25
					  abs(8000*systemFrequency/256/1000)   //   8 s*1.0MHz/256 = 31250.00
				};

				struct Instrument{
					std::string name;
					uint8_t attack_index;
					uint8_t decay_index;
					uint8_t sustain_index;
					uint8_t release_index;
					Instrument(std::string,uint8_t,uint8_t,uint8_t,uint8_t);
				};

				std::vector <Instrument> instruments;
				int active_instrument_index;
				void next_instrument();
				void set_instrument();

			} envelope;
	} voice;

	int graphPointer = 0;
	uint8_t graphBuffer[1000];
	int prevX = 0;
	int prevY = 0;
};


#endif
