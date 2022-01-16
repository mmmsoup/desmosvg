#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

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

int main(int argc, char **argv) {
	xmlDoc *svg;
	svg = xmlReadFile(argv[1], NULL, 0);
	xmlNode *rootNode;
	rootNode = xmlDocGetRootElement(svg);

	double width, height;
	xmlAttr *attrs;
	attrs = rootNode->properties;
	while (attrs) {
		if (strcmp(attrs->name, "width") == 0) {
			if (strToDouble(attrs->children->content, &width)) {
				fprintf(stderr, "invalid value '%s' for width property of svg\n", attrs->children->content);
				exit(1);
			}
		} else if (strcmp(attrs->name, "height") == 0) {
			if (strToDouble(attrs->children->content, &height)) {
				fprintf(stderr, "invalid value '%s' for height property of svg\n", attrs->children->content);
				exit(1);
			}
		}
		attrs = attrs->next;
	}

	// cubic bezier function
	printf("B_{cx}\\left(t, x_1, x_2, x_3, x_4 \\right) = \\left(1-t\\right)^{3}x_{1}+3t\\left(1-t\\right)^{2}x_{2}+3t^{2}\\left(1-t\\right)x_{3}+t^{3}x_{4} \\left\\{ 0 \\le t\\le 1 \\right\\}\n");
	printf("B_{cy}\\left(t, y_1, y_2, y_3, y_4 \\right) = \\left(1-t\\right)^{3}y_{1}+3t\\left(1-t\\right)^{2}y_{2}+3t^{2}\\left(1-t\\right)y_{3}+t^{3}y_{4} \\left\\{ 0 \\le t\\le 1 \\right\\}\n");
	
	iterateNodes(rootNode);

	xmlFreeDoc(svg);
	xmlCleanupParser();
	return 0;
}

// recursively iterate through nodes in xml tree and parse them if necessary
void iterateNodes(xmlNode *node) {
	while (node) {
		parseNode(node);
		iterateNodes(node->children);
		node = node->next;
	}
	return;
}

void parseNode(xmlNode *node) {
	xmlAttr *attrs;
	double x1 = 0, y1 = 0;
	char *cchar;
	double x2, y2;
	double cx1, cy1;
	double cx2, cy2;
	double rx, ry;
	double largearc, sweep, xrotation;
	double m;
	double min, max;
	double temp;
	int rel;
	int firstInstruct;
	enum pathInstruction {NONE, CLOSE='Z', CLOSE_REL='z', MOVETO='M', MOVETO_REL='m', LINETO='L', LINETO_REL='l', HLINETO='H', HLINETO_REL='h', VLINETO='V', VLINETO_REL='v', CBEZIER='C', CBEZIER_REL='c', QBEZIER='Q', QBEZIER_REL='q', ARC='A', ARC_REL='a'} instruct;

	if (strcmp(node->name, "line") == 0) {
		printf("line\n");
	} else if (strcmp(node->name, "path") == 0) {
		attrs = node->properties;
		firstInstruct = 1;
		while (attrs) {
			if (strcmp(attrs->name, "d") == 0) {
				cchar = attrs->children->content;
				while (1) {
					if (*cchar == '\0') break;
					rel = 0;
					while (1) {
						if (*cchar == ' ') {
							cchar++;
						} else {
							if (*cchar != 0x2d && *cchar != 0x2e && (*cchar < 0x2f || *cchar > 0x3a)) {
								instruct = *cchar++;
							} else if (instruct == MOVETO) {
								instruct = LINETO;
							} else if (instruct == MOVETO_REL) {
								instruct = LINETO_REL;
							}
							break;
						}
					}
					switch (instruct) {
						case MOVETO_REL:
							rel = 1;
						case MOVETO:
							temp = strtod(cchar, &cchar);
							x1 = (rel && !firstInstruct) ? x1 + temp : temp;
							if (*cchar == ',') cchar++;
							temp = strtod(cchar, &cchar);
							y1 = (rel && !firstInstruct) ? temp - y1 : -1 * temp;
							firstInstruct = 0;
							break;
						case LINETO_REL:
							rel = 1;
						case LINETO:
							temp = strtod(cchar, &cchar);
							x2 = rel ? x1 + temp : temp;
							if (*cchar == ',') cchar++;
							temp = -1 * strtod(cchar, &cchar);
							y2 = rel ? y1 + temp : temp;
							m = (y2-y1)/(x2-x1);
							if (x2 > x1) {
								min = x1;
								max = x2;
							} else {
								min = x2;
								max = x1;
							}
							printf("y = %fx + %f \\left\\{ %f <= x <= %f \\right\\}\n", m, y1 - m*x1, min, max);
							x1 = x2;
							y1 = y2;
							firstInstruct = 0;
							break;
						case HLINETO_REL:
							rel = 1;
						case HLINETO:
							temp = strtod(cchar, &cchar);
							x2 = rel ? x1 + temp : temp;
							if (x2 > x1) {
								min = x1;
								max = x2;
							} else {
								min = x2;
								max = x1;
							}
							printf("y = %f \\left\\{ %f <= x <= %f \\right\\}\n", y1, min, max);
							x1 = x2;
							firstInstruct = 0;
							break;
						case VLINETO_REL:
							rel = 1;
						case VLINETO:
							temp = -1 * strtod(cchar, &cchar);
							y2 = rel ? y1 + temp : temp;
							if (y2 > y1) {
								min = y1;
								max = y2;
							} else {
								min = y2;
								max = y1;
							}
							printf("x = %f \\left\\{ %f <= y <= %f \\right\\}\n", x1, min, max);
							y1 = y2;
							firstInstruct = 0;
							break;
						case CBEZIER_REL:
							rel = 1;
						case CBEZIER:
							// control point 1
							temp = strtod(cchar, &cchar);
							cx1 = rel ? x1 + temp : temp;
							if (*cchar == ',') cchar++;
							temp = -1 * strtod(cchar, &cchar);
							cy1 = rel ? y1 + temp : temp;

							// conrtol point 2
							if (*cchar == ',') cchar++;
							temp = strtod(cchar, &cchar);
							cx2 = rel ? x1 + temp : temp;
							if (*cchar == ',') cchar++;
							temp = -1 * strtod(cchar, &cchar);
							cy2 = rel ? y1 + temp : temp;

							// finish point
							if (*cchar == ',') cchar++;
							temp = strtod(cchar, &cchar);
							x2 = rel ? x1 + temp : temp;
							if (*cchar == ',') cchar++;
							temp = -1 * strtod(cchar, &cchar);
							y2 = rel ? y1 + temp : temp;

							printf("\\left( B_{cx}\\left( t, %f, %f, %f, %f \\right), B_{cy}\\left( t, %f, %f, %f, %f \\right) \\right)\n", x1, cx1, cx2, x2, y1, cy1, cy2, y2);

							x1 = x2;
							y1 = y2;
							firstInstruct = 0;
							break;
						case ARC_REL:
							rel = 1;
						case ARC:
							// radii
							rx = strtod(cchar, &cchar);
							if (*cchar == ',') cchar++;
							ry = strtod(cchar, &cchar);
							
							// x-axis rotation
							if (*cchar == ',') cchar++;
							xrotation = strtod(cchar, &cchar);
							
							// flags
							if (*cchar == ',') cchar++;
							largearc = strtod(cchar, &cchar);
							if (*cchar == ',') cchar++;
							sweep = strtod(cchar, &cchar);

							// finish point
							if (*cchar == ',') cchar++;
							temp = strtod(cchar, &cchar);
							x2 = rel ? x1 + temp : temp;
							if (*cchar == ',') cchar++;
							temp = -1 * strtod(cchar, &cchar);
							y1 = rel ? y1 + temp : temp;

							x1 = x1;
							y1 = y2;
							firstInstruct = 0;
							break;
						case CLOSE_REL:
						case CLOSE:
							x1 = 0;
							y1 = 0;
							cchar++;
							break;
						default:
							cchar++;
							break;
					}
				}
			}
			attrs = attrs->next;
		}
	}
	return;
}

