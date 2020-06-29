#include "../include/lsbExtract.h"
#include "../include/lsbHelper.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void lsb1ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsbiExtractEncryptedBytes(const uint8_t *bmp, uint8_t *dst, size_t bmpSize, size_t embedSize);


void lsb1Extract(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    lsb1ExtractDataBytes(bmp, dst, size);
    lsb1ExtractExtensionBytes(bmp, dst, size);
}

void lsb1ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte;

    for (size_t i = 0; i < size; i++)
    {
        extractedByte = lsb1ExtractByte(i, bmp);
        dst[i] = extractedByte;
    }
}

void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte = 1;

    for (size_t i = size; extractedByte != 0; i++)
    {
        extractedByte = lsb1ExtractByte(i, bmp);
        dst[i] = extractedByte;
    }
}

void lsb4Extract(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    lsb4ExtractDataBytes(bmp, dst, size);
    lsb4ExtractExtensionBytes(bmp, dst, size);
}

void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    for (size_t i = 0, j = 0; j < size; i += 2, j++)
    {
        uint8_t extractedByte = lsb4ExtractByte(i, bmp);
        dst[j] = extractedByte;
    }
}

void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte = 1;

    //  i = size * 2 because for each byte we want to extract, we need to go through 2 bmp bytes
    for (size_t i = size * 2, j = size; extractedByte != 0; i += 2, j++)
    {
        extractedByte = lsb4ExtractByte(i, bmp);
        dst[j] = extractedByte;
    }
}

void lsbiExtract(const uint8_t *bmp, uint8_t **dst, size_t bmpSize, size_t embedSize)
{
    uint8_t *encMsg = malloc(embedSize);

    lsbiExtractEncryptedBytes(bmp + RC4_KEY_SIZE, encMsg, bmpSize, embedSize);

    *dst = RC4(encMsg, bmp, embedSize);
}

void lsbiExtractEncryptedBytes(const uint8_t *bmp, uint8_t *dst, size_t bmpSize, size_t embedSize)
{
    size_t hop = getHop(bmp[0]);
    size_t cursor = SIZE_BYTES * 8 * hop, laps = 0;

    for (size_t i = 0; i < embedSize; i++)
    {
        uint8_t extractedByte = lsbiExtractByte(bmp, bmpSize, &cursor, &laps, hop);
        dst[i] = extractedByte;
    }
}