#ifndef VOICE_H
#define VOICE_H
#include <cstdint>
#include <math.h>
#include <iostream>
typedef uint8_t byte;
typedef uint16_t word;
typedef unsigned int reg24;

/*
Voice Class
*/
class Voice
{
public:
    /*
     Ebvelope Class
     */
    struct Envelope
    {
        
        public:
            Envelope();
        
            void write_ATTACK_DECAY(byte value);
            void write_SUSTAIN_RELEASE(byte value);
    } Envelope;

    /*
    Wave Class
    */
    struct Wave
    {
        public:
        
            Wave();
        
            // Frequence
            void write_FREQ_LO(byte value);
            void write_FREQ_HI(byte value);
            // Pulse Width
            void write_PW_LO(byte value);
            void write_PW_HI(byte value);
        
            // Control Register: Waveform, Sync, Gate...
            void write_CONTROLREG(byte value);
        
            void reset();
        
            void clock();
        
        protected:
        
            // frequency
            word freq;
            // pulse with
            word pw;
            // counter of cycles
            reg24 accumulator;
        
            byte value;
        
            // High 4 bit define waveForm
            // 4: Triangle, 5: Saw; 6: Rectangle: 7: Noise
            byte waveForm;
        
            bool sync;
            bool gate;
            bool ring;
            bool test;
        
    } Wave;
    
    byte output();
    void play();
    
	Voice();
};

#endif