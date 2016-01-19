#include "sidwrapper.h"
#include <stdio.h>

int main(int argc, char* argv[]){
        char mem[0x10000][9];

	struct SID* sid = newSID(&mem, true);

	printf("Hello");
	getchar();
	return 0;
}
