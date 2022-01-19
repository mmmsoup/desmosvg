#include "options.h"

#include <stdio.h>

void getOptions(int argc, char **argv, char *path, int *pathLen, options *opts) {
	int c, i;

	opts->js = 0;
	opts->replace = 0;
	opts->clear = 0;
	opts->colour = -1;
	
	while (c != -1) {
		c = getopt_long(argc, argv, "c:fhjrx", longOptions, &i);
		
		switch (c) {
			case 'c':
				int col = 0;
				int digit;
				if (*optarg == '#') optarg++;
				for (i = 0; i < 6; i++) {
					if (*optarg < 0x67 && *optarg > 0x60) digit = *optarg - 0x57;
					else if (*optarg < 0x47 && *optarg > 0x40) digit = *optarg - 0x37;
					else if (*optarg < 0x3a && *optarg > 0x29) digit = *optarg - 0x30;
					else {
						fprintf(stderr, "error reading colour code\n");
						exit(1);
					}
					optarg++;
					col += digit << (4*(5-i));
				}
				opts->colour = col;
				// needs to be javascript to set custom colour
				opts->js = 1;
				break;
			case 'f':
				// generate folder id
				break;
			case 'h':
				printf("hmm it sure would be helpful if i wrote the help message\n");
				exit(0);
			case 'j':
				opts->js = 1;
				break;
			case 'r':
				opts->replace = 1;
				break;
			case 'x':
				opts->clear = 1;
				break;
			case -1:
				break;
			default:
				printf("never should have come here\n");
				exit(1);
				break;
		}
	}

	if (optind < argc)
    {
		strcpy(path, argv[optind]);
		*pathLen = strlen(argv[optind]);
		if (optind + 1 < argc) {
			fprintf(stderr, "too many non-option arguments\n");
			exit(1);
		}
    } else {
		fprintf(stderr, "missing input file path\n");
		exit(1);
	}

	return;
}
