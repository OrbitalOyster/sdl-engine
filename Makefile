# Generic Makefile

# Working directories
SRC_DIR := src
BIN_DIR := bin

# Executable name
EXE := engine

OUTPUT := $(BIN_DIR)/$(EXE)
WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Warith-conversion \
						-Wfloat-equal -Werror
STANDART := -std=c11
OPTIMIZATION := -O2
# Debugging info
DEBUG := -ggdb3
# Preprocessor flags (example -DDEBUG -DLOG)
DFLAGS := -DCOLOR_OUTPUT

CC := gcc
CFLAGS := $(WARNINGS) $(STANDART) $(OPTIMIZATION) $(DFLAGS) $(DEBUG)
LDLIBS := -lm

# All .c files
C_FILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)

# All .o files
OBJS := $(C_FILES:.c=.o)

# Final result
all: $(BIN_DIR) $(OUTPUT)
$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $(OUTPUT)

# Executable directory
$(BIN_DIR):
	mkdir $(BIN_DIR)

# Compile all .c files
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# Header dependencies
-include $(C_FILES:.c=.d)

# Run executable
run:
	./$(OUTPUT)

# Clear working directory
clean:
	-rm $(SRC_DIR)/*.o
	-rm $(SRC_DIR)/**/*.o
	-rm $(SRC_DIR)/*.d
	-rm $(SRC_DIR)/**/*.d
	-rm $(OUTPUT)

.PHONY: all run clean
