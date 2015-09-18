#ifndef UTILS_H
#define UTILS_H
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>


namespace Utils
{
	template<class T> 
	std::string hexify(T val)
	{
		std::stringstream stream;

		// Cast to int is necessary to print '\0' for uint8_t (which is a typedef of char)!
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << (int)val;
		return stream.str();
	}
};

#endif