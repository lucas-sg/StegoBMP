#include "../include/lsbExtract.h"
#include "../include/lsbEmbed.h"
#include "../include/lsbHelper.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
int getHopFromBmpFile(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
void lsb1ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
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
        extractedByte = 0;

        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t extractedBit = bmp[i * 8 + j] & 1u;
            extractedByte |= (uint8_t) (extractedBit << (7u - j));
        }

        dst[i] = extractedByte;
    }
}

void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte = 1;

    for (size_t i = size; extractedByte != 0; i++)
    {
        extractedByte = 0;

        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t extractedBit = bmp[i * 8 + j] & 1u;
            extractedByte |= (uint8_t) (extractedBit << (7u - j));
        }

        dst[i] = extractedByte;
    }
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
        extractedByte = 0;

        uint8_t firstFourBits  = bmp[i]   & 0x0Fu;
        uint8_t secondFourBits = bmp[i+1] & 0x0Fu;

        extractedByte |= (uint8_t) (firstFourBits << 4u);
        extractedByte |= secondFourBits;
        dst[j] = extractedByte;
    }
}

void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte = 1;

    //  i = size * 2 because we need to extract 2 bmp bytes to get 1 byte
    for (size_t i = size * 2, j = size; extractedByte != 0; i += 2, j++)
    {
        extractedByte = 0;

        uint8_t firstFourBits  = bmp[i]   & 0x0Fu;
        uint8_t secondFourBits = bmp[i+1] & 0x0Fu;
        extractedByte         |= (uint8_t) (firstFourBits << 4u);
        extractedByte         |= secondFourBits;
        dst[j]                 = extractedByte;
    }
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

size_t lsbiExtractBytes(const uint8_t *source, size_t sourceSize, uint8_t *dst, int hop)
{
    int cursor = 0;
    int laps = 0;

    uint32_t msgSize = lsbiExtractSize(source, sourceSize, hop, &cursor, &laps);
    printf("El msg size es %d\n", msgSize);
    for (int i = 0; i < msgSize; i++)
    {
        uint8_t byte = 0;

        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t sourceByte = source[cursor] & 1;
            byte |= (sourceByte & 1) << (7 - j);

            cursor += hop;
            if (cursor == sourceSize)
            {
                cursor = ++laps;
            }
            else if (cursor > sourceSize)
            {
                cursor %= sourceSize;
                laps++;
            }
        }
        dst[i] = byte;
    }
    return msgSize;
}

/**
 * This functions is fucking wrong, encrypted size is needed to this function to work and extract the size
 * but it also calculates it here. Size of stego thing that was embed MUST be different from the encryptedSize.
 * 
 * The extraction is also NOT working, it is extracting things without any sense
 */ 
void lsbiExtractAndDecrypt(const uint8_t *source, uint8_t *dst, size_t sourceSize, size_t encryptedSize)
{
    int hop = source[0] == 0 ? 256 : source[0];
    uint8_t *encMsg = malloc(encryptedSize);
    size_t encryptedMessageSize = lsbiExtractBytes(source + 6, sourceSize - 6, encMsg, hop);
    printf("MGS SIZE %d\n", encryptedMessageSize);
    uint8_t *decrypted = RC4(encMsg, source, encryptedMessageSize);
    for (int i = 0 ; i < 102 ;i++)
        printf("%c", decrypted[i]);
    printf("\n");
    memcpy(dst, decrypted, encryptedMessageSize);
}

uint8_t *lsbiExtract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    int currentHopIndex = 0;
    int rowCursor = widthInBytes - 1;
    int hop = 256;
    const uint8_t *rc4Key = extractRC4Key(bmpFile, bmpSize, widthInBytes);
    uint8_t *stego = malloc(stegoSize + 1);
    int stegoCount = stegoSize - 1;

    while (currentHopIndex < hop && stegoCount >= 0)
    {
        bmpCursor = bmpSize - 1;
        rowCursor = widthInBytes - 7 - currentHopIndex;
        while (bmpCursor >= 0 && stegoCount >= 0)
        {
            uint8_t auxByte;
            while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
            {
                if (rowCursor < 0)
                {
                    int diff = hop + rowCursor;
                    rowCursor = widthInBytes - 1 - diff;
                    bmpCursor -= widthInBytes;

                    if (bmpCursor < 0)
                    {
                        break;
                    }
                }
                uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 0);
                auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);

                if (rowCursor <= 0)
                {
                    rowCursor = widthInBytes - 1;
                    bmpCursor -= widthInBytes;
                }
                else
                {
                    rowCursor -= hop;
                }
            }
            stego[stegoIndex++] = auxByte;
            stegoBitCursor = 7;
            stegoCount--;
        }
        currentHopIndex++;
    }
    stego[stegoSize] = 0;

    return stego;
}

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    uint8_t *rc4Key = malloc(6 * sizeof(uint8_t));           // extract 6 to another constant
    size_t firstPixelIndex = bmpSize - 1 - widthInBytes - 1; // extract 3 to another constant
    memcpy(rc4Key, bmpFile - firstPixelIndex, 6);
    return rc4Key;
}

//FIXME THIS IS BROKEN
unsigned int extractDecimalFromBinary(uint8_t binary)
{
    int decimal = 0, i = 0, rem;
    while (binary != 0)
    {
        rem = binary % 10;
        binary /= 10;
        decimal += rem * pow(2, i);
        ++i;
    }
    return decimal;
}