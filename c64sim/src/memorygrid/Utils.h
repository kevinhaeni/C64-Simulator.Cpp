#ifndef UTILS_H
#define UTILS_H
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <locale>


namespace Utils
{

	template<class T> 
	std::string hexify(T val)
	{
		std::stringstream stream;

		// Cast to int is necessary to print '\0' for uint8_t (which is a typedef of char)!
		stream << std::setfill('0') << std::setw(sizeof(T) * 2)	<< std::hex << (int)val;
		std::string str = stream.str();
		std::locale settings;
		std::string converted = "0x";

		for (short i = 0; i < str.size(); ++i)
			converted += (std::toupper(str[i], settings));

		return converted;
	}

	template<class T>
	std::string stringify(T val)
	{
		std::stringstream stream;

		// Cast to int is necessary to print '\0' for uint8_t (which is a typedef of char)!
		stream << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << (int)val;
		std::string str = stream.str();
		std::locale settings;
		std::string converted;

		for (short i = 0; i < str.size(); ++i)
			converted += (std::toupper(str[i], settings));

		return converted;
	}



	// combine two bytes to make a word
	uint16_t makeWord(uint8_t lsb, uint8_t msb);
};

#endif