#ifndef GRAPH_H
#define GRAPH_H
#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <cmath>
#include <string>
#include <stack>

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

	void exit();
	SDL_AudioSpec* getSpec();

	struct Voice{
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
	} voice;


	int graphPointer = 0;
	uint8_t graphBuffer[1000];
	int prevX = 0;
	int prevY = 0;

};


#endif
