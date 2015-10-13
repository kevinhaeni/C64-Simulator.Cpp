#ifndef ENVELOPE_H
#define ENVELOPE_H

/*
Envelope Class
*/
class Envelope
{

public:
    Envelope();
    
    void write_ATTACK_DECAY(uint8_t value);
    void write_SUSTAIN_RELEASE(uint8_t value);
};

#endif