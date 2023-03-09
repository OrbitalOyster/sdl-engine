CC = gcc

OUTPUT = game

WARNINGS = -Wall -Wextra -Werror -Wshadow -Wpedantic
OPTIMIZATION = -O2
STANDART = -std=c11
CFLAGS = ${OPTIMIZATION} ${WARNINGS} ${STANDART}
LDFLAGS = -o ${OUTPUT}
OBJ = main.o

all: ${OUTPUT}

${OUTPUT}: ${OBJ} 
	${CC} ${OBJ} ${LDFLAGS} ${CFLAGS}

clean:
	rm *.o

