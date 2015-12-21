#ifndef SID_H
#define SID_H
#include "SDL.h"
#include "SDL_audio.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include "Utils.h"

typedef char memory[0x10000][9];

/* Window Constants */


/* Audio Constants */
const int SAMPLING_RATE = 44100;				// number of samples per second

#define TTF_ENABLED 1							// disable TTF on systems without sdl_ttf extension
#define DEBUG 0

#ifdef TTF_ENABLED
#include "SDL_ttf.h"
#endif

class SID
{
public:
	SDL_Window *window;

	// SDL audio stuff
	SDL_AudioSpec desiredDeviceSpec;
	SDL_AudioSpec spec;
	SDL_AudioDeviceID dev;

	unsigned long  timeSinceLastInterval = 0;
	int refreshInterval = 5;				         // mseconds
	int refreshCounter = 0;
	int thread_exit = 0;
	bool pause_thread = false;	
	SDL_Renderer *renderer;

#ifdef TTF_ENABLED
	TTF_Font* font;
#endif

	// properties
	bool showWindow = false;
	bool keyGpressed;
	memory* _mem;
	int graphDisplayLength = 9900;

	// Graph members
	int graphPointer = 0;
	uint8_t* graphBuffer;		// size must be a multiple of the window width (default 1980)
	int graphBufferSize = SAMPLING_RATE;

	std::ofstream logFile;
	std::ifstream* envFile;

	int logCounter = 0;
	uint8_t volume;
	uint8_t getVolume();

	struct Instrument{
		std::string name;
		uint8_t attack_index;
		uint8_t decay_index;
		uint8_t sustain_index;
		uint8_t release_index;
		Instrument(std::string, uint8_t, uint8_t, uint8_t, uint8_t);
	};

	struct Voice{
		Voice();

		bool silent = false;
		// WaveForm parameters
		enum WaveForm{
			SINE = 1, RECT = 2, SAWTOOTH = 3, TRIANGLE = 4, NOISE = 5
		} waveForm;
		int frequency;              // the frequency of the voice

		double pwn;            // Square wave width, 0 - 1.0
		double maxWaveValue;  //
		long double phase;
		long double phaseInc;

		int previousWaveValue; // needed for Filter
		int activeWaveValue;
		int getPreviousWaveValue();
		int getActiveWaveValue();

		bool ring;
		bool sync;
		bool filter;

		bool isRing();
		bool isSync();
		bool isFilter();

		int getFrequency();
		void setFrequency(int freq);
		void setPWN(double pwn);

		// SDL buffer handling members
		int audioPosition = 0;      // counter

		double getWaveValue();
		double getEnvelopeValue();

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

		} envelope;
	} voice1, voice2, voice3;

	struct Filter{
		Filter();
		uint16_t cutoff;
		uint8_t resonance;

		uint8_t calcLowPass(Voice* voice);


		int filterValues[2000];

		enum FilterMode{
			LOWPASS, BANDPASS, HIGHPASS, VOICETHREEOFF
		} mode;
	} filter;

	std::vector <Instrument> instruments;
	int active_instrument_index = 0;

	// methods
	SID(memory* mem, bool window);
	void init();

	void drawGraph(SDL_Renderer *renderer);

	void dispatchEvent(SDL_Event* event);
	void exit();
	uint8_t readMemory(uint16_t addr) const;
	uint8_t readMemoryUpper4Bit(uint16_t addr) const;
	uint8_t readMemoryLower4Bit(uint16_t addr) const;

	uint16_t readMemory2Register(uint16_t addr) const;
	uint16_t readMemoryVoiceFrequency(uint16_t addr) const;
	double readMemoryVoicePWN(uint16_t addr) const;

	void writeMemory(uint8_t value, uint16_t addr) const;
	void writeMemoryUpper4Bit(uint8_t value, uint16_t addr) const;
	void writeMemoryLower4Bit(uint8_t value, uint16_t addr) const;

	char* readMemoryBitwise(uint16_t addr) const;
	void updateRegisters();


	// SDL audio members
	SDL_AudioSpec* getSpec();


	void setVoiceFromControlReg(Voice* voice, char reg[]);

	
	void resetGraphBuffer();

};


#endif
