#ifndef VOICE_H
#define VOICE_H

#include "Wave.h"
#include "Envelope.h"

/*
Voice Class
*/
class Voice
{
protected:
    Wave wave;
    Envelope envelope;
public:

	Voice();
    
    // allows SID to acces protected Objects/ Values
    friend class SID;
};

#endif