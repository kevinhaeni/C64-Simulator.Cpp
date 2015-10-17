// C64.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include "C64.h"
#include "WaveGenerator.h"
#include "SDL.h"
#include "MemoryGrid.h"

#include <thread>

#define DEBUG 0

C64 theC64;
void soundTest() {
	WaveGenerator* waveGenerator = new WaveGenerator();;

	int duration = 1000;
	double Hz = 600;
	waveGenerator->playSound(Hz, duration);
	waveGenerator->wait();
}


//
//int main(int argc, char* argv[])
//{
//
//
//	//theC64.run();
//    //theC64.test();
//	//soundTest();
//
//	// init a memory grid window
//	//std::thread t(&MemoryGrid::init, grid);
//	//MemoryGrid *grid = new MemoryGrid(&theC64);
//
//	// put some data into memory
//
//	while (true){
//
//		for (uint16_t i = 0; i < MEMSIZE; i++){
//			//theC64.writeMemory(i, (uint8_t)rand());
//			theC64.writeMemory(i, (uint8_t)rand());
//				std::this_thread::sleep_for(std::chrono::microseconds(1));
//		}
//	}
//		
//	int a;
//	std::cin >> a;
//	return 0;
//}
