#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOLDER_ID_SIZE 32

typedef struct {
	int js; // generate javascript to be pasted into browser console
	// options that only apply if js is set to 1
	char folderID[FOLDER_ID_SIZE+1]; // put equations in a folder with ID of this value if not null
	int replace; // replace folder if id matches
	int clear; // clear current graph
	int colour; // recolour svg with rgb colour 0xRRGGBB (value of -1 -> don't recolour)
} options;

int strToDouble(char*, double*);
void outputEquation(char*, int, int, options*);

#endif
