// C64.cpp : Defines the entry point for the console application.
//
#pragma once
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "c64/C64.h"

int _tmain(int argc, _TCHAR* argv[])
{

	C64 theC64;
	theC64.test();

	int a;
	std::cin >> a;
	return 0;
}

