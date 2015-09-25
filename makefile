all:
	g++ -std=c++11 C64Sim.cpp Utils.cpp C64.cpp CPU.cpp Flag.cpp Memory.cpp SID.cpp Instruction.cpp -o C64Sim
