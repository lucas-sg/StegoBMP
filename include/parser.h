#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "types.h"

PARSE_RET parseInput(int argc, char *argv[], UserInput *params);
EXTENSION_CHECK checkForBMPExtension(const char *fileName);
size_t sizeOfUserInputStruct();

#endif