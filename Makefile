# Generic Makefile

# Working directories
SRC_DIR := src
INCLUDE_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Executable name
EXE := engine

# Compiler options
OUTPUT := $(BIN_DIR)/$(EXE)
WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Warith-conversion \
						-Wfloat-equal -Werror
STANDART := -std=c11
OPTIMIZATION := -O2
# Debugging info
DEBUG := -ggdb3
# Preprocessor flags (example -DDEBUG -DLOG)
DFLAGS := -DDEBUG_MSG -DCOLOR_OUTPUT

CC := gcc
CFLAGS := $(WARNINGS) $(STANDART) $(OPTIMIZATION) $(DFLAGS) $(DEBUG) \
					-I $(INCLUDE_DIR)
# Linker libraries (example -lm)
LDLIBS := -lm -lSDL2 -lSDL2_ttf

# All source subdirectories
SRC_SDIRS := $(SRC_DIR)/ $(wildcard $(SRC_DIR)/*/) $(wildcard $(SRC_DIR)/**/*/)

# All .c files
C_FILES := $(foreach d, $(SRC_SDIRS), $(wildcard $(d)*.c))

# All .o files
OBJS := $(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(C_FILES:.c=.o))
OBJ_SDIRS := $(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(SRC_SDIRS))

# All .d files
DEPS := $(OBJS:.o=.d)

# Final result
all: $(OUTPUT)

# Testing
include dtest/Makefile

# Object subdirectories and files
$(OUTPUT): $(OBJ_SDIRS) $(OBJS) $(BIN_DIR)
	$(CC) $(OBJS) $(LDLIBS) -o $(OUTPUT)

$(OBJ_SDIRS):
	mkdir -p $(OBJ_SDIRS)

# Executable directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile all .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# Header dependencies
-include $(DEPS)

# Run executable
run: $(OUTPUT)
	./$(OUTPUT)

# Clear working directory
clean:
	-rm $(OBJ_DIR)/* -r
	-rm $(OUTPUT)

.PHONY: all run test clean
