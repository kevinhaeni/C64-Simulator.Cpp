// C wrapper for SID C++ class
#ifndef __SIDWRAPPER_H
#define __SIDWRAPPER_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct SID SID;
typedef char memory[0x10000][9];

SID* newSID(memory*, int interval, bool showWindow);

#ifdef __cplusplus
}
#endif

#endif
