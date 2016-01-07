// C wrapper for SID C++ class
#ifndef __SIDWRAPPER_H
#define __SIDWRAPPER_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct SID SID;
typedef char memtype[0x10000][9];

SID* newSID(memtype*, int showWindow);
void* getWindow(SID* sid);
void SIDUpdateRegisters(SID* sid);
void dispatchEvent(SID* sid, void* ev);

#ifdef __cplusplus
}
#endif

#endif
