# Generic Makefile

OUTPUT := game
WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Warith-conversion \
						-Wfloat-equal -Werror
STANDART := -std=c11
OPTIMIZATION := -O2
DFLAGS := -DDEBUG_MSG -DCOLOR_OUTPUT -DGEOMETRY_DEBUG
DEBUG := -ggdb3

CC := gcc
CFLAGS := $(WARNINGS) $(STANDART) $(OPTIMIZATION) $(DFLAGS) $(DEBUG)
LDLIBS := -lm -lSDL2 -lSDL2_ttf

# All .c files
C_FILES := $(wildcard *.c) $(wildcard **/*.c)

# All .o files
OBJS := $(C_FILES:.c=.o)

# Final result
all: $(OUTPUT)
$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $(OUTPUT)

# Compile all .c files
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# Header dependencies
-include $(C_FILES:.c=.d)

# Clear working directory
clean:
	-rm *.o
	-rm **/*.o
	-rm *.d
	-rm **/*.d
	-rm $(OUTPUT)

valgrind:
	valgrind --leak-check=full --leak-resolution=high --show-leak-kinds=all \
		--track-origins=yes  --gen-suppressions=all --suppressions=sdl.supp \
		./$(OUTPUT)

.PHONY: all clean valgrind

