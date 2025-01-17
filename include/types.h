#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

#define SUCCEEDED 1
#define FAILED    0

typedef enum PARSE_RET
{
    PARSED_OK = 0,
    EMBED_AND_EXTRACT_ERROR,
    MISSING_PARAMETER,
    MISSING_ARGUMENT,
    NOT_BMP_FILE_ERROR,
    NOT_STEG_OPTION_ERROR,
    NOT_ENC_OPTION_ERROR,
    NOT_MODE_OPTION_ERROR,
    MISSING_ACTION
} PARSE_RET;
typedef enum OPTION
{
    EMBD = 0,
    EXTR,
    INPUT,
    OUTPUT,
    CARRIER,
    STEG,
    ALGO,
    MODE,
    PASS,
    END
} OPTION;
typedef enum ACTION
{
    EMBED,
    EXTRACT
} ACTION;
typedef enum STEGO_ALGO
{
    LSB1,
    LSB4,
    LSBI
} STEGO_ALGO;
typedef enum EXTENSION_CHECK
{
    BMP_OK,
    NOT_BMP_EXTENSION_ERROR,
    SMALL_FILE_NAME_ERROR
} EXTENSION_CHECK;
typedef enum ENCRYPTION
{
    NONE,
    AES_128,
    AES_192,
    AES_256,
    DES
} ENCRYPTION;
typedef enum ENC_MODE
{
    ECB,
    CFB,
    OFB,
    CBC
} ENC_MODE;

struct UserInput
{
    ACTION action;
    char *inputFileName;
    char *outputFileName;
    char *carrierFileName;
    STEGO_ALGO stegoAlgorithm;
    ENCRYPTION encryption;
    ENC_MODE mode;
    uint8_t *password;
    PARSE_RET status;
};
typedef struct UserInput UserInput;

typedef enum EXTRACT_RET
{
    EXTRACTION_SUCCEEDED,
    WRONG_STEGO_ALGO_ERROR
} EXTRACT_RET;

#endif
