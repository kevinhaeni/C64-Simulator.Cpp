SIDTest: sidwrapper.o SID.o
	g++ SIDTest.c sidwrapper.o SID.o Utils.o -o SIDTest -lSDL2 -lSDL2_ttf

sidwrapper.o: SID.o
	 g++ -c -o sidwrapper.o sidwrapper.c -std=c++11 -I /usr/include/SDL2

SID.o: Utils.o
	g++ -c -o SID.o Utils.o SID.cpp -std=c++11 -I /usr/include/SDL2

Utils.o:
	g++ -c -o Utils.o Utils.cpp -std=c++11
