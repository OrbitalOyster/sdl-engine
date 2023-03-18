CC = gcc

OUTPUT = game

WARNINGS = -Wall -Wextra -Wpedantic -Wshadow -Warith-conversion -Wfloat-equal -Werror
OPTIMIZATION = -O2
STANDART = -std=c11
CFLAGS = ${OPTIMIZATION} ${WARNINGS} ${STANDART}
LDLIBS = -lSDL2
LDFLAGS = -o ${OUTPUT}
OBJ = main.o core.o dmath/dmath.o geometry/point.o geometry/ortholine.o geometry/orthosegment.o geometry/orthorect.o

all: ${OUTPUT}

${OUTPUT}: ${OBJ}
	${CC} ${OBJ} ${LDLIBS} ${LDFLAGS} ${CFLAGS}

clean:
	rm *.o
	rm geometry/*.o
	rm dmath/*.o

