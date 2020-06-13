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

typedef enum OPTION {
    EMBED = 0,
    EXTRACT,
    INPUT,
    OUTPUT,
    CARRIER,
    STEG,
    ALGO,
    MODE,
    PASS,
    END
} OPTION;
// typedef enum ACTION {
//     EMBED,
//     EXTRACT
// } ACTION;
typedef enum STEGO_ALGO {
    LSB1,
    LSB2,
    LSBI
} STEGO_ALGO;
typedef enum EXTENSION_CHECK {
    BMP_OK,
    NOT_BMP_EXTENSION_ERROR
} EXTENSION_CHECK;

struct UserInput {
    // enum ACTION action;
    char *inputFileName;
    char *outputFileName;
    char *carrierFileName;
    enum STEGO_ALGO algorithm;
    enum PARSE_RET parsingStatus;
};
typedef struct UserInput UserInput;

PARSE_RET parseInput(int argc, char *argv[], UserInput *params);
EXTENSION_CHECK checkForBMPExtension(const char *fileName);
size_t sizeOfUserInputStruct();

#endif