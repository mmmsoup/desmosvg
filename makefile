CFLAGS=`xml2-config --cflags`
LIBS=`xml2-config --libs`

all: desmosvg

desmosvg: main.o util.o options.o svg.o
	gcc ${CFLAGS} \
		main.o util.o options.o svg.o \
		-o desmosvg \
		${LIBS}

main.o: main.c main.h util.o
	gcc -c ${CFLAGS} main.c ${LIBS}

util.o: util.c util.h
	gcc -c util.c

options.o: options.c options.h util.o
	gcc -c options.c

svg.o: svg.c svg.h util.o
	gcc -c ${CFLAGS} svg.c ${LIBS}

clean:
	rm *.o
