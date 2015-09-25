all:
	g++ -std=c++11 C64/C64Sim.cpp C64/Utils.cpp C64/C64.cpp C64/CPU.cpp C64/Flag.cpp C64/Memory.cpp C64/SID.cpp C64/Instruction.cpp -o C64Sim
