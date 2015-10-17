#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "Graph.h"

SDL_AudioSpec desiredDeviceSpec;
SDL_AudioSpec spec;
SDL_AudioDeviceID dev;

uint8_t graph[44100];
int graphPointer = 0;
//Graph* g = new Graph(graph);;

struct SDLVoice{
	int frequency;				// the frequency of the voice
	int amp;					// the amplitude of the voice

	int samplesLeft;			// number of samples to be played, eg: 1.2 seconds * 44100 samples per second
	int audioPosition = 0;		// counter

	void playForNMicroSeconds(int n){		 // calculates the number of samples and delays the program
		samplesLeft = 44100 * n / 1000;
		audioPosition = 0;					 // reset the counter
		SDL_PauseAudioDevice(dev, 0);		 // play
		SDL_Delay(n);						 // delay the program to prevent the voice to be overridden before it has been played to the end
		SDL_PauseAudioDevice(dev, 1);		 // pause		

	}

	enum WaveForm{
		SINE, RECT, SAWTOOTH, TRIANGLE
	} waveForm;

    
    uint8_t getSample(){
        
        int time = (audioPosition * frequency) / 44100;
        double step = 44100.0 / (double)frequency;
        
        uint8_t rect_value = 0x00;
        
        //std::cout << step << " - ";
        switch (waveForm){
            case SINE:
            {
                float sineStep = 2 * M_PI * audioPosition * frequency / 44100;
                return amp * sin(sineStep) + 127;		// +127 because otherwise the negative values will be cut off
                break;
            }
            case RECT:
                if(fmod((double)audioPosition, step) >= 0.5 * step){
                    rect_value = 0xFF;
                }
                return amp * rect_value;
                break;
                
            case SAWTOOTH:
                return amp * fmod((double)audioPosition, step );
                break;
                
            case TRIANGLE:
                return amp * abs(fmod((double)audioPosition, step) - 0.5 * step) ;
                break;
                
            default:
                return 0;
        }
    }
} voice;


// SDL calls this function whenever it wants its buffer to be filled with samples
void SDLAudioCallback(void *data, Uint8 *buffer, int length){
	uint8_t *stream = (uint8_t*)buffer;
	
	for (int i = 0; i <= length; i++){

		if (voice.samplesLeft-- <= 0)
			stream[i] = 0;
		else
		{
			stream[i] = voice.getSample();
			voice.audioPosition++;

			
			// Graph
			if (graphPointer < 44099)
				graph[graphPointer++] = stream[i];

		}		


	}
}

void initSDL(){
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
	SDL_zero(desiredDeviceSpec);

	desiredDeviceSpec.freq = 44100;			// Sample Rate
	desiredDeviceSpec.format = AUDIO_U8;	// Unsigned 8-Bit Samples
	desiredDeviceSpec.channels = 1;			// Mono
	desiredDeviceSpec.samples = 2048;		// The size of the Audio Buffer (in number of samples, eg: 2048 * 1 Byte (AUDIO_U8)
	desiredDeviceSpec.callback = SDLAudioCallback;

	dev = SDL_OpenAudioDevice(NULL, 0, &desiredDeviceSpec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (dev == 0) {
		printf("\nFailed to open audio: %s\n", SDL_GetError());
	}
	else {
		SDL_PauseAudioDevice(dev, 1); /* pause! */
		SDL_PauseAudio(1);
	}
}

int main(int argc, char* argv[]){
	initSDL();

	Graph* g = new Graph(graph);

    voice.waveForm = SDLVoice::WaveForm::TRIANGLE;
    voice.amp = 3;

    voice.frequency = 440;
    voice.playForNMicroSeconds(1000);

	int i;
	std::cin >> i;

	return 0;
}