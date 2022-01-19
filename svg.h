#ifndef SVG_H
#define SVG_H

#include <libxml/parser.h>
#include <string.h>

#include "util.h"

void svg(char*, options*);
void iterateNodes(xmlNode*, options*);
void parseNode(xmlNode*, options*);

#endif
