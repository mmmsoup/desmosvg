#include "main.h"

int main(int argc, char **argv) {
	options opts;
	char *path = (char*)malloc(512);
	int pathSize;

	getOptions(argc, argv, path, &pathSize, &opts);

	path = realloc(path, pathSize);

	if (opts.js) {
		strcpy(opts.folderID, "abcdefghijklmnopqrstuvwxyzabcdef");
		printf("state = Calc.getState(); ");
		if (opts.clear) printf("state.expressions.list = []; ");
	} else {
		memset(opts.folderID, 0, sizeof(opts.folderID));
	}

	svg(path, &opts);

	if (opts.js) {
		printf("Calc.setState(state);\n");
	}

	free(path);

	return 0;
}
