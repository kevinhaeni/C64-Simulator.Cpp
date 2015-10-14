#include "SID.h"

SID::SID()
{

}

byte SID::read_byte(uint16_t adrLong){
    byte adr = adrLong & 0xFF;
    switch (adr) {
        case 0x19:
            // Pot X
            break;
        case 0x1A:
            // Pot Y
            break;
        case 0x1B:
            //return voice3.envelope.output();
            break;
        case 0x1C:
            //return voice3.wave.output();
            break;
        default:
            return 0x00;
            break;
    }
    return 0x00;
}

void SID::write_byte(word adrLong, byte value){
    
    byte adr = adrLong & 0xFF;
    switch (adr) {
        case 0x00:
            voice1.wave.write_FREQ_LO(value);
            break;
        case 0x01:
            voice1.wave.write_FREQ_HI(value);
            break;
        case 0x02:
            voice1.wave.write_PW_LO(value);
            break;
        case 0x03:
            voice1.wave.write_PW_HI(value);
            break;
        case 0x04:
            voice1.wave.write_CONTROLREG(value);
            break;
        case 0x05:
            voice1.envelope.write_ATTACK_DECAY(value);
            break;
        case 0x06:
            voice1.envelope.write_SUSTAIN_RELEASE(value);
            break;
        case 0x07:
            voice2.wave.write_FREQ_LO(value);
            break;
        case 0x08:
            voice2.wave.write_FREQ_HI(value);
            break;
        case 0x09:
            voice2.wave.write_PW_LO(value);
            break;
        case 0x0a:
            voice2.wave.write_PW_HI(value);
            break;
        case 0x0b:
            voice2.wave.write_CONTROLREG(value);
            break;
        case 0x0c:
            voice2.envelope.write_ATTACK_DECAY(value);
            break;
        case 0x0d:
            voice2.envelope.write_SUSTAIN_RELEASE(value);
            break;
        case 0x0e:
            voice3.wave.write_FREQ_LO(value);
            break;
        case 0x0f:
            voice3.wave.write_FREQ_HI(value);
            break;
        case 0x10:
            voice3.wave.write_PW_LO(value);
            break;
        case 0x11:
            voice3.wave.write_PW_HI(value);
            break;
        case 0x12:
            voice3.wave.write_CONTROLREG(value);
            break;
        case 0x13:
            voice3.envelope.write_ATTACK_DECAY(value);
            break;
        case 0x14:
            voice3.envelope.write_SUSTAIN_RELEASE(value);
            break;
        case 0x15:
            filter.write_FC_LO(value);
            break;
        case 0x16:
            filter.write_FC_HI(value);
            break;
        case 0x17:
            filter.write_RES_FILT(value);
            break;
        case 0x18:
            filter.write_MODE_VOL(value);
            break;
        default:
            break;
    }
}

void SID::updateRegisters(){
    
}

void SID::emulateCycles(){
    
}
