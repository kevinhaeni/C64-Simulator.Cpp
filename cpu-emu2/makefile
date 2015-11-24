# makefile for cpu-emu2
# 2015/BHO1
# initial
# TODO: make a blody cp4students target/script
#
# 22.4.2012/BHO1
# for compiling on linux:
# sudo apt-get install build-essential make libsdl-image1.2-dev
# sudo apt-get install cdecl

#name of alu main test program

CC        := gcc
LD        := ld

MODULES   := cpu printers alu test ui
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(filter-out $(sdir)/alu.student.c, $(wildcard $(sdir)/*.c)))

#SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
#SCR       := $(filter-out $(SRC_DIR)/alu/alu.student.c, $(SRC))
OBJ       := $(patsubst src/%.c,build/%.o,$(SRC))
#INCLUDES  := $(addprefix -I,src/include)
INCLUDES  := $(addprefix -I,$(SRC_DIR))

#alu
ALU_MODULES   := printers alu test ui
ALU_SRC_DIR   := $(addprefix src/,$(ALU_MODULES))
ALU_BUILD_DIR := $(addprefix build/,$(ALU_MODULES))

ALU_SRC       := $(foreach sdir,$(ALU_SRC_DIR),$(filter-out $(sdir)/alu.student.c, $(wildcard $(sdir)/*.c)))

ALU_OBJ       := $(patsubst src/%.c,build/%.o,$(ALU_SRC))
ALU_INCLUDES  := $(addprefix -I,$(ALU_SRC_DIR))



#vpath %.c $(ALU_SRC_DIR)

vpath %.c $(SRC_DIR)

define make-goal
$1/%.o: %.c
	@echo $< 
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all alu alutest checkdirs clean

all: checkdirs build/test.exe

alu: checkdirs build/alu.exe

build/test.exe: $(OBJ)
	$(CC) -o $@ $^ 

build/alu.exe: $(ALU_OBJ)
	$(CC) -o $@ $^ 


checkdirs: $(BUILD_DIR)

# $(ALU_BUILD_DIR)

#$(ALU_BUILD_DIR):
#	@mkdir -p $@

$(BUILD_DIR):
	@mkdir -p $@

alutest: checkdirs build/alu.exe
	@echo "\n running alutest"	 $@	$^ 
	cat test/alu/alu-test-minimal.txt | build/alu.exe  | diff - test/alu/alu-test-minimal-results.txt

clean:
	@echo "\n ram -rf $(ALU_BUILD_DIR) $(BUILD_DIR) running alu-test minimal" $@	$^ 
	@rm -rf $(ALU_BUILD_DIR)
	@rm -rf $(BUILD_DIR)

# ALU_BUILD_DIR   := $(BUILD_DIR)
$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
