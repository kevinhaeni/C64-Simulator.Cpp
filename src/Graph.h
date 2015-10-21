#ifndef GRAPH_H
#define GRAPH_H
#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <cmath>
#include <string>
#include <stack>

/* Constants */
const int REFRESH_INTERVAL = 50;				// mseconds
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 255;
const std::string WINDOW_TITLE = "Wave Graph";

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

public:
	Graph();
	void init();
	void mainLoop();
	void drawGraph();

	void exit();


	struct Voice{
		int frequency;				// the frequency of the voice
		int amp;					// the amplitude of the voice

		int audioLength;			// number of samples to be played, eg: 1.2 seconds * 44100 samples per second
		int audioPosition = 0;		// counter

		void playForNMicroSeconds(int n);

		enum WaveForm{
			SINE = 0, RECT = 1, SAWTOOTH = 2, TRIANGLE = 3
		} waveForm;


		uint8_t getSample();
	} voice;
	int graphPointer = 0;
	uint8_t graphBuffer[1000];


};


#endif