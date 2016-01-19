#include "memorygridwrapper.h"
#include <stdio.h>

int main(int argc, char* argv[]){
        char mem[0x10000][9];

	struct MemoryGrid* memgrid = newMemoryGrid(&mem);

	printf("Hello");
	getchar();
	return 0;
}
