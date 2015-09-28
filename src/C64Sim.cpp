// C64.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include "C64.h"

#define DEBUG 0

int main(int argc, char* argv[])
{

	C64 theC64;
	theC64.run();

	int a;
	std::cin >> a;
	return 0;
}

