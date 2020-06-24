#ifndef EMBED_H
#define EMBED_H

#include <string.h>
#include "lsbExtract.h"
#include "lsbEmbed.h"
#include "fileParser.h"
#include "types.h"

typedef struct
{
    uint8_t *data;
    size_t   size;
} OUTPUT_BMP;

OUTPUT_BMP *embed(UserInput userInput, BMP *carrierBmp, MESSAGE *msg);
int encrypt(const uint8_t *plaintext, int plaintextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode,
            const uint8_t *password);
OUTPUT_BMP *lsb1Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);
OUTPUT_BMP *lsb4Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);
OUTPUT_BMP *lsbiEmbed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath);
size_t buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer);

#endif
