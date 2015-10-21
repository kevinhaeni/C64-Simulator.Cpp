#include "Voice.h"

Voice::Voice()
{
}

byte Voice::output(){
    
}

/* Wave */
Voice::Wave::Wave()
{
    
}

void Voice::Wave::write_FREQ_LO(byte freq_lo)
{
    freq = freq & 0xff00 | freq_lo & 0x00ff;
}

void Voice::Wave::write_FREQ_HI(byte freq_hi)
{
    freq = (freq_hi << 8) & 0xff00 | freq & 0x00ff;
}

void Voice::Wave::write_PW_LO(byte pw_lo)
{
    pw = pw & 0xff00 | pw_lo & 0x00ff;
}

void Voice::Wave::write_PW_HI(byte pw_hi)
{
    pw = (pw_hi << 8) & 0xff00 | pw & 0x00ff;
}

void Voice::Wave::write_CONTROLREG(byte controlByte){
    waveForm = controlByte >> 4 & 0x0F;
    
    gate = static_cast<bool>(controlByte & 0x01);
    sync = static_cast<bool>(controlByte & 0x02);
    ring = static_cast<bool>(controlByte & 0x04);
    test = static_cast<bool>(controlByte & 0x08);
}

void Voice::Wave::reset(){
    
    freq = 0;
    pw = 0;
    
    waveForm = 0;
    
    sync = false;
    gate = false;
    ring = false;
    test = false;
}

void Voice::Wave::clock(){
    //if model == ntsc
    double actualFreq =  this->freq * 14318182 / (14 * pow(2,24));
    // else model == pal
    // byte actualFreq =  this->freq * 17734472 Hz / (18 * pow(2,24));
    
    double step = 44100.0 / actualFreq;
    
    switch (this->waveForm) {
            // Triangle
        case 0x01:
            this->value =  fabs(fmod((double)this->accumulator, step) - 0.5 * step) ;
            break;
            // Saw
        case 0x02:
            this->value = fmod((double)this->accumulator, step );
            break;
            // Pulse
        case 0x04:
            byte rect_value;
            if(fmod((double)this->accumulator, step) >= 0.5 * step){
                rect_value = 0xFF;
            }
            this->value = rect_value;
            break;
            // Noise
        case 0x08:
            break;
        default:
            break;
    }
    
    // add cycle
    this->accumulator++;
}

/* Envelope */
Voice::Envelope::Envelope()
{
    
}

void Voice::Envelope::write_ATTACK_DECAY(byte value){};
void Voice::Envelope::write_SUSTAIN_RELEASE(byte value){};