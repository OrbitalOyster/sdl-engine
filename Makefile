# Generic Makefile

# Working directories
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

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
# Preprocessor flags(example - DDEBUG - DLOG)
DFLAGS := -DDEBUG_MSG -DCOLOR_OUTPUT

CC := gcc
CFLAGS := $(WARNINGS) $(STANDART) $(OPTIMIZATION) $(DFLAGS) $(DEBUG)
LDLIBS := -lm

# All source subdirectories
SUBDIRS := $(SRC_DIR)/ $(wildcard $(SRC_DIR)/*/) $(wildcard $(SRC_DIR)/**/*/)

# All.c files
C_FILES := $(foreach d,$(SUBDIRS),$(wildcard $(d)*.c))

# All.o files
OBJS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(C_FILES:.c=.o))
OBJ_SUBDIRS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%, $(SUBDIRS))

# All.d files
DEPS := $(OBJS:.o=.d)

# Final result
all: $(OUTPUT)

# Object subdirectories and files
$(OUTPUT): $(OBJ_SUBDIRS) $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $(OUTPUT)

$(OBJ_SUBDIRS):
	mkdir $(OBJ_SUBDIRS)

# Executable directory
$(BIN_DIR):
	mkdir $(BIN_DIR)

# Compile all.c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# Header dependencies
-include $(DEPS)

# Run executable
run:
	./$(OUTPUT)

# Clear working directory
clean:
	-rm $(OBJ_DIR) -r
	-rm $(OUTPUT)

.PHONY: all run clean
