// C64.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include "C64.h"
#include "WaveGenerator.h"
#include "SDL.h"

#define DEBUG 0


void soundTest() {
	WaveGenerator* waveGenerator = new WaveGenerator();;

	int duration = 1000;
	double Hz = 600;
	waveGenerator->playSound(Hz, duration);
	waveGenerator->wait();
}

int main(int argc, char* argv[])
{

	C64 theC64;
	//theC64.run();
    //theC64.test();
	soundTest();
    
	int a;
	std::cin >> a;
	return 0;
}
