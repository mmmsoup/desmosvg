#include "util.h"

// return 0 if conversion was successful, else 1. store return value in 'ret'
int strToDouble(char *str, double *ret) {
	char *end;
	double doub;
	doub = strtod(str, &end);
	if (*end == '\0') {
		*ret = doub;
		return 0;
	} else {
		return 1;
	}
}

void outputEquation(char *latex, int colour, int id, options *opts) {
	if (opts->js) {
		char colStr[19] = "color: \"#000000\", ";
		char val;
		if (opts->colour != -1) {
			for (int i = 0; i < 6; i++) {
				val = (opts->colour >> 4*(5-i)) & 0xf;
				colStr[9+i] = val <= 9 ? val+48 : val+87;
			}
		} else if (colour != -1) {
			for (int i = 0; i < 6; i++) {
				val = (colour >> 4*(5-i)) & 0xf;
				colStr[9+i] = val <= 9 ? val+48 : val+87;
			}
		} else memset(colStr, 0, 19);

		char folderIDStr[FOLDER_ID_SIZE+15];
		if (opts->folderID[0] == '\0') {
			memset(folderIDStr, 0, FOLDER_ID_SIZE+15);
		} else {
			sprintf(folderIDStr, "folderId: \"%s\", ", opts->folderID);
		}

		printf("state.expressions.list.push({type: \"expression\", %s%slatex: \"", colStr, folderIDStr);
		char c;
		while (1) {
			c = *latex;
			if (c == '\\') {
				printf("\\\\");
			} else if (c == '\0') {
				break;
			} else {
				printf("%c", c);
			}
			latex++;
		}
		printf("\"}); ");
	} else {
		printf("%s\n", latex);
	}
}
