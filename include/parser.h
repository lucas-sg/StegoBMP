#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

typedef enum PARSE_RET {
    PARSED_OK,
    EMBED_AND_EXTRACT_ERROR,
    MISSING_PARAMETER,
    MISSING_ARGUMENT,
    NOT_BMP_FILE_ERROR,
    MISSING_ACTION
} PARSE_RET;
typedef enum EXTENSION_CHECK EXTENSION_CHECK;
typedef struct UserInput UserInput;

PARSE_RET parseInput(int argc, char *argv[], UserInput *params);
EXTENSION_CHECK checkForBMPExtension(const char *fileName);
size_t sizeOfUserInputStruct();

#endif