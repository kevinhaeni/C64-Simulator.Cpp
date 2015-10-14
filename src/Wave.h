#ifndef WAVE_H
#define WAVE_H
#include <cstdint>

typedef uint8_t byte;
typedef uint16_t word;

/*
Wave Class
*/
class Wave
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
    
    byte output();

protected:
    word freq;
    word pw;
    
    // High 4 bit define waveForm
    // 4: Triangle, 5: Saw; 6: Rectangle: 7: Noise
    byte waveForm;
    
    bool sync;
    bool gate;
    bool ring;
    bool test;
};

#endif