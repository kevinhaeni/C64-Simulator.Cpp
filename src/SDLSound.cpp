#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "Graph.h"

SDL_AudioSpec desiredDeviceSpec;
SDL_AudioSpec spec;
SDL_AudioDeviceID dev;


//Graph* g = new Graph(graph);;
//
//
//
//void initSDL(){
//	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
//	SDL_zero(desiredDeviceSpec);
//
//	desiredDeviceSpec.freq = 44100;			// Sample Rate
//	desiredDeviceSpec.format = AUDIO_U8;	// Unsigned 8-Bit Samples
//	desiredDeviceSpec.channels = 1;			// Mono
//	desiredDeviceSpec.samples = 2048;		// The size of the Audio Buffer (in number of samples, eg: 2048 * 1 Byte (AUDIO_U8)
//	desiredDeviceSpec.callback = SDLAudioCallback;
//
//	dev = SDL_OpenAudioDevice(NULL, 0, &desiredDeviceSpec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
//	if (dev == 0) {
//		printf("\nFailed to open audio: %s\n", SDL_GetError());
//	}
//	else {
//		SDL_PauseAudioDevice(dev, 1); /* pause! */
//		SDL_PauseAudio(1);
//	}
//}
//
//int main(int argc, char* argv[]){
//	initSDL();
//
//	Graph* g = new Graph(graph);
//
//    voice.waveForm = SDLVoice::WaveForm::TRIANGLE;
//    voice.amp = 3;
//
//    voice.frequency = 440;
//    voice.playForNMicroSeconds(1000);
//
//	int i;
//	std::cin >> i;
//
//	return 0;
//}