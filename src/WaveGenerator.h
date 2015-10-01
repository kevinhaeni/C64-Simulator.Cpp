#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"
#include <stdio.h>
#include <math.h>

#define AMPLITUDE 28000			// the frequency we want
#define FREQUENCY 44100			// the frequency we want

struct SoundObject
{
	double freq;
	int samplesLeft;
};

class WaveGenerator{
private:
	SDL_AudioSpec wantSpec;
	SDL_AudioSpec spec;
	SDL_AudioDeviceID dev;
	double v;
	std::queue<SoundObject> soundobjects;

	//unsigned int audio_pos;		// which sample we are up to
	//int audio_len;				// how many samples left to play, stops when <= 0 
	//float audio_frequency;		// audio frequency in cycles per sample 
	//float audio_volume;			// audio volume, 0 - ~32000/

public:
	WaveGenerator();
	~WaveGenerator();
	void init();
	void playSound(double freq, int duration);
	void generateSamples(Sint16 *stream, int length);
	void wait();
};

void audio_callback(void*, Uint8*, int);