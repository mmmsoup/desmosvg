CFLAGS="`xml2-config --cflags`"

all: main.c main.h
	gcc ${CFLAGS} main.c -o desmosvg `xml2-config --libs`
