#ifndef EMBED_H
#define EMBED_H

#include <stdint.h>
#include <string.h>
#include "lsbExtract.h"
#include "lsbEmbed.h"
#include "./fileParser.h"
#include "types.h"

typedef struct
{
    uint8_t *data;
    size_t   size;
} OUTPUT_BMP;

OUTPUT_BMP *embed(UserInput userInput, BMP *carrierBmp, MESSAGE *msg);
OUTPUT_BMP *lsb1Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);
OUTPUT_BMP *lsb4Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);
OUTPUT_BMP *lsbiEmbed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);
size_t buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer);

#endif
