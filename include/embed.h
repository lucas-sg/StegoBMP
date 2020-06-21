#ifndef EMBED_H
#define EMBED_H

#include <stdint.h>
#include <string.h>
#include "../stego/lsbDecrypt.h"
#include "../stego/lsbEncrypt.h"
#include "./bmpParser.h"
#include "types.h"

typedef struct
{
    uint8_t *data;
    size_t size;
} OUTPUT_BMP;

OUTPUT_BMP *embed(uint8_t *carrierBmp, size_t carrierSize, const char *msgPath, size_t msgSize,
                  UserInput userInput, const uint8_t *msg, const char *bmpPath);
OUTPUT_BMP *lsb1Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);

#endif
