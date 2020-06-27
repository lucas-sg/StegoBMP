#include "../include/lsbExtract.h"
#include "../include/lsbEmbed.h"
#include "../include/lsbHelper.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
int getHopFromBmpFile(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
void lsb1ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
uint8_t lsb1ExtractByte(size_t byteIndex, const uint8_t *bmp);
void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
uint8_t lsb4ExtractByte(size_t byteIndex, const uint8_t *bmp);
unsigned int extractDecimalFromBinary(uint8_t binary);
/**
 * Keep in mind these functions could be abstracted a lot
 */

// Extract size bytes from dst
void lsb1ExtractBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
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

uint8_t lsb1ExtractByte(size_t byteIndex, const uint8_t *bmp)
{
    uint8_t extractedByte = 0;

    for (uint8_t j = 0; j < 8; j++)
    {
        uint8_t extractedBit = bmp[byteIndex * 8 + j] & 1u;
        extractedByte |= (uint8_t) (extractedBit << (7u - j));
    }

    return extractedByte;
}

void lsb4ExtractBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    lsb4ExtractDataBytes(bmp, dst, size);
    lsb4ExtractExtensionBytes(bmp, dst, size);
}

void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte;

    for (size_t i = 0, j = 0; j < size; i += 2, j++)
    {
        extractedByte = lsb4ExtractByte(i, bmp);
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

uint8_t lsb4ExtractByte(size_t byteIndex, const uint8_t *bmp)
{
    uint8_t extractedByte = 0;

    uint8_t firstFourBits  = bmp[byteIndex]   & 0x0Fu;
    uint8_t secondFourBits = bmp[byteIndex+1] & 0x0Fu;

    extractedByte |= (uint8_t) (firstFourBits << 4u);
    extractedByte |= secondFourBits;

    return extractedByte;
}

uint32_t lsbiExtractSize(const uint8_t *source, size_t sourceSize, int hop, int *cursor, int *laps)
{
    int auxCursor = *cursor, auxLaps = *laps;
    uint8_t msgBytes[4];
    for (int i = 0; i < 4; i++)
    {
        uint8_t byte = 0;
        // Needing 8 destination bytes per source byte.
        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t sourceByte = source[auxCursor] & 1;
            byte |= (sourceByte & 1) << (7 - j);

            auxCursor += hop;
            if (auxCursor == sourceSize)
            {
                auxCursor = ++auxLaps;
            }
            else if (auxCursor > sourceSize)
            {
                auxCursor %= sourceSize;
                auxLaps++;
            }
        }
        msgBytes[i] = byte;
    }
    *cursor = auxCursor;
    *laps = auxLaps;

    return extractStegoSizeFrom(msgBytes);
}

void lsbiExtractEncryptedBytes(const uint8_t *bmp, uint8_t *dst, size_t bmpSize, size_t embedSize, size_t hop)
{
    size_t cursor = SIZE_BYTES * 8 * hop;

    for (size_t i = 0; i < embedSize; i++)
    {
        uint8_t extractedByte = lsbiExtractByte(bmp, bmpSize, hop, &cursor);
        dst[i] = extractedByte;
    }
}

void lsbiExtract(const uint8_t *bmp, uint8_t **dst, size_t bmpSize, size_t embedSize)
{
    size_t hop = bmp[0] == 0 ? 256 : bmp[0];
    uint8_t *encMsg = malloc(embedSize);

    //tamaño || RC4(msg)
    //msg = tamaño || datos || ext
    //msg = tamaño || openssl(tamaño || datos || ext)

    lsbiExtractEncryptedBytes(bmp + 6, encMsg, bmpSize, embedSize, hop);

    *dst = RC4(encMsg, bmp, embedSize);
}

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    uint8_t *rc4Key = malloc(6 * sizeof(uint8_t));           // extract 6 to another constant
    size_t firstPixelIndex = bmpSize - 1 - widthInBytes - 1; // extract 3 to another constant
    memcpy(rc4Key, bmpFile - firstPixelIndex, 6);
    return rc4Key;
}