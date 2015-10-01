#include "WaveGenerator.h"

WaveGenerator::WaveGenerator(){
	init();
}

WaveGenerator::~WaveGenerator(){
	SDL_CloseAudio();
}


void WaveGenerator::init(){
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);	
	SDL_zero(wantSpec);

	wantSpec.freq = FREQUENCY;
	wantSpec.format = AUDIO_S16SYS;
	wantSpec.channels = 1;		// mono
	wantSpec.samples = 2048;
	wantSpec.callback = audio_callback;
	wantSpec.userdata = this;

	dev = SDL_OpenAudioDevice(NULL, 0, &wantSpec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (dev == 0) {
		printf("\nFailed to open audio: %s\n", SDL_GetError());
		// throw exception

	}
	else{
		SDL_PauseAudio(0);	
	}
	
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
			stream[i] = AMPLITUDE * std::sin(v * 2 * M_PI / FREQUENCY);
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

	SDL_LockAudio();
	soundobjects.push(so);
	SDL_UnlockAudio();
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


void audio_callback(void *_waveGenerator, Uint8 *_stream, int _length)
	Sint16 *stream = (Sint16*)_stream;
	int length = _length / 2;
	WaveGenerator* waveGenerator = (WaveGenerator*)_waveGenerator;

	waveGenerator->generateSamples(stream, length);
}

void main(){


	voice testVoiceA;
	voice testVoiceB;
	voice testVoiceC;
	testVoiceA.volume = 1;
	testVoiceB.volume = 1;
	testVoiceC.volume = 1;
	testVoiceA.pan = 0.5;
	testVoiceB.pan = 0;
	testVoiceC.pan = 1;
	testVoiceA.phase = 0;
	testVoiceB.phase = 0;
	testVoiceC.phase = 0;
	testVoiceA.frequency = getFrequency(45);// A3
	testVoiceB.frequency = getFrequency(49);// C#4
	testVoiceC.frequency = getFrequency(52);// E4
	Uint16 C0waveformLength = getWaveformLength(0);
	testVoiceA.waveformLength = C0waveformLength;
	testVoiceB.waveformLength = C0waveformLength;
	testVoiceC.waveformLength = C0waveformLength;
	float sineWave[C0waveformLength];
	buildSineWave(sineWave, C0waveformLength);
	testVoiceA.waveform = sineWave;
	testVoiceB.waveform = sineWave;
	testVoiceC.waveform = sineWave;

	init();

	SDL_Delay(42);	// give it some time

	SDL_PauseAudioDevice(dev, 0);// unpause audio.

	bool running = true;

	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				running = SDL_FALSE;
			}
		}
		for (i = 0; i<samplesPerFrame; i++) audioBuffer[audioMainLeftOff + i] = 0;

		//printf("audioMainLeftOff___________%5d\n", audioMainLeftOff);

		speak(&testVoiceA);
		speak(&testVoiceB);
		speak(&testVoiceC);

		if (audioMainAccumulator > 1) {
			for (i = 0; i<samplesPerFrame; i++) {
				audioBuffer[audioMainLeftOff + i] /= audioMainAccumulator;
			}
		}
		audioMainAccumulator = 0;

		audioMainLeftOff += samplesPerFrame;
		if (audioMainLeftOff == audioBufferLength) audioMainLeftOff = 0;

		mainAudioLead = audioMainLeftOff - SDL_AtomicGet(&audioCallbackLeftOff);
		if (mainAudioLead < 0) mainAudioLead += audioBufferLength;
		//printf("mainAudioLead:%5d\n", mainAudioLead);
		if (mainAudioLead < floatStreamLength) printf("An audio collision may have occured!\n");
		SDL_Delay(mainAudioLead*syncCompensationFactor);
	}
	
	// exit function (cleanup stuff)

	return 0;
}