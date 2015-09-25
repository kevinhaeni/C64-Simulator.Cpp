all:
	g++ -std=c++11 src/C64Sim.cpp src/Utils.cpp src/C64.cpp src/CPU.cpp src/Flag.cpp src/Memory.cpp src/SID.cpp src/Instruction.cpp -o C64Sim
