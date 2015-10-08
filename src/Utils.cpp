#include "Utils.h"

namespace Utils{
	uint16_t makeWord(uint8_t lsb, uint8_t msb) {
		uint16_t word = (msb << 8 | lsb);
		return word;
	}
}