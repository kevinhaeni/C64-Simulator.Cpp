#ifndef FLAG_H
#define FLAG_H
#include <cstdint>

class Flag
{
private:

	uint8_t value;
public:
	Flag();

	void set();
	void clear();
	uint8_t get();

};

#endif