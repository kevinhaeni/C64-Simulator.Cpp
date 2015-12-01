SIDTest: sidwrapper.o Graph.o
	g++ SIDTest.c sidwrapper.o Graph.o -o SIDTest -lSDL2 -lSDL2_ttf

sidwrapper.o: Graph.o
	 g++ -c -o sidwrapper.o sidwrapper.c -std=c++11 -I /usr/include/SDL2

Graph.o:
	g++ -c -o Graph.o Graph.cpp -std=c++11 -I /usr/include/SDL2

