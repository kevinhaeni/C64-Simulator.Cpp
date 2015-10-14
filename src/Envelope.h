#ifndef ENVELOPE_H
#define ENVELOPE_H
#include <cstdint>

typedef uint8_t byte;
typedef uint16_t word;

/*
Envelope Class
*/
class Envelope
{

public:
    Envelope();
    
    void write_ATTACK_DECAY(byte value);
    void write_SUSTAIN_RELEASE(byte value);
};

#endif