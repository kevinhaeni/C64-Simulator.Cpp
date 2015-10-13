#ifndef WAVE_H
#define WAVE_H


/*
Wave Class
*/
class Wave
{ 
public:

	Wave();
    
    // Frequence
    void write_FREQ_LO(uint8_t vlue);
    void write_FREQ_HI(uint8_t value);
    // Pulse Width
    void write_PW_LO(uint8_t value);
    void write_PW_HI(uint8_t value);
    
    // Control Register: Waveform, Sync, Gate...
    void write_CONTROLREG(uint8_t value);
    

protected:
    uint16_t freq;
    uint16_t pw;
    
    bool sync;
    bool gate;
    bool ring;
    bool test;
};

#endif