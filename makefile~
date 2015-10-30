#Detect OS
OS := $(shell uname)

#CPP_FILES specifies which files to compile as part of the project
CPP_FILES := $(wildcard src/*.cpp)

#OBJ_FILES
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

#CC specifies which compiler we're using
CC := g++

#CC specifies which compiler version we're using
CV := c++11

ifeq ($(OS),Darwin)
	# Define more Header File Paths
	LIB := -framework SDL2 -I/Library/Frameworks/SDL2.framework/Versions/A/Headers  -I/Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers

	# Compiler Flags: specifies the additional compilation options we're using
	# -w suppresses all warnings
	CC_FLAGS := -w
	LD_FLAGS :=
else
	# Define stuff for linux
	LIB :=
	CC_FLAGS := -I /usr/include/SDL2 -Wall -g -w
	LD_FLAGS := -lSDL2 -lSDL2_ttf

endif
	

C64Sim: $(OBJ_FILES)
	g++  -o $@ $^ $(LD_FLAGS) 
obj/%.o: src/%.cpp
	$(CC) -std=$(CV)  $(OBJS) $(LIB) $(CC_FLAGS)  -c -o $@ $< 
