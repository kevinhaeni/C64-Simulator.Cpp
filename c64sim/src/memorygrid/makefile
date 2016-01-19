MemoryGridTest: memorygridwrapper.o MemoryGrid.o
	g++ MemoryGridTest.c memorygridwrapper.o MemoryGrid.o Utils.o -o MemoryGridTest -lSDL2 -lSDL2_ttf

memorygridwrapper.o: MemoryGrid.o
	 g++ -c -o memorygridwrapper.o memorygridwrapper.c -std=c++11 -I /usr/include/SDL2

MemoryGrid.o: Utils.o
	g++ -c -o MemoryGrid.o Utils.o MemoryGrid.cpp -std=c++11 -I /usr/include/SDL2

Utils.o:
	g++ -c -o Utils.o Utils.cpp -std=c++11
