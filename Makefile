# Generic Makefile

OUTPUT := game
WARNINGS := -Wall -Wextra -Wshadow -Wconversion -Warith-conversion -Wfloat-equal -Werror
STANDART := -std=c11
OPTIMIZATION := -O2
DEBUG :=

# Implicit variables
CC := gcc
CFLAGS := $(WARNINGS) $(STANDART) $(OPTIMIZATION) $(DEBUG)
LDLIBS := -lSDL2

# All .c files
C_FILES := $(wildcard *.c) $(wildcard **/*.c)

# TODO: Header dependencies

# All .o files
OBJS := $(C_FILES:.c=.o)

# Final result
all: $(OUTPUT)
$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $(OUTPUT)

# Compile all .c files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clear working directory
clean:
	-rm *.o
	-rm **/*.o
	-rm $(OUTPUT)

.PHONY: all clean
