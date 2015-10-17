#include "WaveGenerator.h"

WaveGenerator::WaveGenerator(){
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
	SDL_zero(wantSpec);

	wantSpec.freq = FREQUENCY;			// Sample Rate?
	wantSpec.format = AUDIO_S16;
	wantSpec.channels = 1;				// Mono
	wantSpec.samples = 2048;			// Sample Frame Size
	wantSpec.callback = audio_callback;
	wantSpec.userdata = this;

	dev = SDL_OpenAudioDevice(NULL, 0, &wantSpec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (dev == 0) {
		printf("\nFailed to open audio: %s\n", SDL_GetError());
		// throw exception

	}
	else {
		SDL_PauseAudio(0);
	}
}

WaveGenerator::~WaveGenerator(){
	SDL_CloseAudio();
}


void WaveGenerator::generateSamples(Sint16 *stream, int length){

	int i = 0;
	while (i < length) {

		if (soundobjects.empty()) {
			while (i < length) {
				stream[i] = 0;
				i++;
			}
			return;
		}
		SoundObject& bo = soundobjects.front();

		int samplesToDo = std::min(i + bo.samplesLeft, length);
		bo.samplesLeft -= samplesToDo - i;

		while (i < samplesToDo) {
			stream[i] = AMPLITUDE * sin(v * 2 * M_PI / FREQUENCY);
			i++;
			v += bo.freq;
		}

		if (bo.samplesLeft == 0) {
			soundobjects.pop();
		}
	}
}


void WaveGenerator::playSound(double freq, int duration){
	SoundObject so;
	so.freq = freq;
	so.samplesLeft = duration * FREQUENCY / 1000;

	//SDL_LockAudio();
	SDL_PauseAudioDevice(dev, 1); /* pause! */

	soundobjects.push(so);
	//SDL_UnlockAudio();
	SDL_PauseAudioDevice(dev, 0); /* play! */

}

void WaveGenerator::wait(){
	int size;
	do {
		SDL_Delay(20);
		SDL_LockAudio();
		size = soundobjects.size();
		SDL_UnlockAudio();
	} while (size > 0);
}

// SDL calls this function whenever it wants its buffer to be filled with samples
void audio_callback(void *_waveGenerator, Uint8 *_stream, int _length){
	Sint16 *stream = (Sint16*)_stream;
	int length = _length / 2;
	WaveGenerator* waveGenerator = (WaveGenerator*)_waveGenerator;

	waveGenerator->generateSamples(stream, length);
}
