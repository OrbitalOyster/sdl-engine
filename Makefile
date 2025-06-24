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
WARNINGS := -W
STANDART := -std=c11
OPTIMIZATION := -O2
# Debugging info
DEBUG := -ggdb3
# Preprocessor flags (example -DDEBUG -DLOG)
DFLAGS := -DDEBUG_MSG -DCOLOR_OUTPUT

CC := clang
CFLAGS := $(WARNINGS) $(STANDART) $(OPTIMIZATION) $(DFLAGS) $(DEBUG) \
					-I $(INCLUDE_DIR)
# Linker libraries (example -lm)
LDLIBS := -lSDL3 -lSDL3_image -lSDL3_ttf

# All source subdirectories
SRC_SDIRS := $(SRC_DIR)/ \
						 $(wildcard $(SRC_DIR)/*/) $(wildcard $(SRC_DIR)/**/*/)
INCLUDE_SDIRS := $(INCLUDE_DIR)/ \
								 $(wildcard $(INCLUDE_DIR)/*/) $(wildcard $(INCLUDE_DIR)/**/*/)

# All .c files
C_FILES := $(foreach d, $(SRC_SDIRS), $(wildcard $(d)*.c))

# All .h files
H_FILES := $(foreach d, $(INCLUDE_SDIRS), $(wildcard $(d)*.h))

# All .o files
OBJS := $(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(C_FILES:.c=.o))
OBJ_SDIRS := $(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(SRC_SDIRS))

# All .d files
DEPS := $(OBJS:.o=.d)

# Final result
all: $(OUTPUT)

# Object subdirectories and files
$(OUTPUT): $(OBJ_SDIRS) $(OBJS) $(BIN_DIR)
	$(CC) $(OBJS) $(LDLIBS) -o $(OUTPUT)

$(OBJ_SDIRS):
	mkdir -p $(OBJ_SDIRS)

# Executable directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile all .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# Header dependencies
-include $(DEPS)

# Run executable
run: $(OUTPUT)
	./$(OUTPUT)

format:
	clang-format $(C_FILES) -i
	clang-format $(H_FILES) -i

valgrind:
	valgrind --leak-check=full --leak-resolution=high --show-leak-kinds=all \
		--track-origins=yes  --gen-suppressions=all \
		./$(OUTPUT)

# Clear working directory
clean:
	-rm $(OBJ_DIR)/* -r
	-rm $(OUTPUT)

.PHONY: all run format valgrind test clean
