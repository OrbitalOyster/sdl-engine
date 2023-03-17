CC = gcc

OUTPUT = game

WARNINGS = -Wall -Wextra -Wpedantic -Wshadow -Warith-conversion -Wfloat-equal -Werror
OPTIMIZATION = -O2
STANDART = -std=c11
CFLAGS = ${OPTIMIZATION} ${WARNINGS} ${STANDART}
LDFLAGS = -o ${OUTPUT}
OBJ = main.o dmath/dmath.o

all: ${OUTPUT}

${OUTPUT}: ${OBJ} 
	${CC} ${OBJ} ${LDFLAGS} ${CFLAGS}

clean:
	rm *.o

