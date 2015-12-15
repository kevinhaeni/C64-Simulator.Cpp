// C wrapper for SID C++ class
#ifndef __MEMORYGRIDWRAPPER_H
#define __MEMORYGRIDWRAPPER_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct MemoryGrid MemoryGrid;
typedef char memtype[0x10000][9];

MemoryGrid* newMemoryGrid(memtype*);

#ifdef __cplusplus
}
#endif

#endif
