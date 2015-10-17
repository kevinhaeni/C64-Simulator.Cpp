#Detect OS
OS := $(shell uname)

#OBJS specifies which files to compile as part of the project
OBJS = src/C64Sim.cpp src/Utils.cpp src/C64.cpp src/CPU.cpp src/Flag.cpp src/Memory.cpp src/SID.cpp src/Instruction.cpp src/Voice.cpp src/WaveGenerator.cpp
#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#CC specifies which compiler version we're using
CV = c++11

#OBJ_NAME specifies the name of our exectuable
OUTPUT = C64Sim

ifeq ($(OS),Darwin)
	# Define more Header File Paths
	LIB = -framework SDL2 -I/Library/Frameworks/SDL2.framework/Versions/A/Headers  -I/Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers
endif
	

all:
	$(CC) -std=$(CV) $(OBJS) $(LIB)  $(COMPILER_FLAGS)  -o $(OUTPUT)
