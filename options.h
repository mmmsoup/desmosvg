#ifndef OPTIONS_H
#define OPTIONS_H

#include <getopt.h>

#include "util.h"

void getOptions(int, char**, char*, int*, options*);

static struct option longOptions[] = {
	{"colour",		required_argument, 0, 'c'},
	{"folder",		no_argument, 0, 'f'},
	{"help",		no_argument, 0, 'h'},
	{"javascript",	no_argument, 0, 'j'},
	{"replace",		no_argument, 0, 'r'},
	{"clear",		no_argument, 0, 'x'},
	{0, 0, 0, 0}
};

#endif
