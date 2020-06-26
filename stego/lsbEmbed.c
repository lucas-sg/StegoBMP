#include "../include/lsbEmbed.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

uint8_t assignNthBitOfXtoY(uint8_t x, uint8_t y, int n);

void lsb1EmbedBytes(const uint8_t *src, uint8_t *dst, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        uint8_t byteToEmbed = src[i];

        // Needing 8 destination bytes per source byte.
        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t dstByte = dst[i * 8 + j];
            dst[i * 8 + j] = assignNthBitOfXtoY(byteToEmbed, dstByte, 7 - j);
        }
    }
}

void lsb4EmbedBytes(const uint8_t *src, uint8_t *dst, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        uint8_t byteToEmbed = src[i];

        uint8_t firstDstByte = dst[i * 2] & 0xF0;
        uint8_t secondDstByte = dst[i * 2 + 1] & 0xF0;

        firstDstByte |= (byteToEmbed & 0xF0) >> 4;
        secondDstByte |= byteToEmbed & 0x0F;

        dst[i * 2] = firstDstByte;
        dst[i * 2 + 1] = secondDstByte;
    }
}

void lsbiEncryptAndEmbed(const uint8_t *src, uint32_t msgSize, uint8_t *dst, size_t dstSize)
{
    int hop = dst[0] == 0 ? 255 : dst[0];
    uint8_t *encSrc = RC4(src, dst, msgSize);
    lsbiEmbedBytes(encSrc, msgSize, dst + 6, dstSize - 6, hop);
}

void lsbiEmbedSize(uint32_t msgSize, uint8_t *dst, size_t dstSize, size_t hop, int *cursor, int *laps)
{
    int auxCursor = *cursor, auxLaps = *laps;
    uint8_t sizeBytes[4] = {0};
    for (int i = 0; i < 4; ++i)
        sizeBytes[i] = ((uint8_t *)&msgSize)[3 - i];

    for (int i = 0; i < 4; i++)
    {
        uint8_t byteToEmbed = sizeBytes[i];

        for (uint8_t j = 0; j < 8; j++)
        {
            dst[auxCursor] = assignNthBitOfXtoY(byteToEmbed, dst[auxCursor], 8 - j - 1);

            auxCursor += hop;
            if (auxCursor == dstSize)
            {
                auxCursor = ++auxLaps;
            }
            else if (auxCursor > dstSize)
            {
                auxCursor %= dstSize;
                auxLaps++;
            }
        }
    }
    *cursor = auxCursor;
    *laps = auxLaps;
}

void lsbiEmbedBytes(const uint8_t *src, size_t msgSize, uint8_t *dst, size_t dstSize, size_t hop)
{
    int cursor = 0;
    int laps = 0;
    lsbiEmbedSize(msgSize, dst, dstSize, hop, &cursor, &laps);

    for (int i = 0; i < msgSize; i++)
    {
        uint8_t byteToEmbed = src[i];

        // Needing 8 destination bytes per source byte.
        for (uint8_t j = 0; j < 8; j++)
        {
            dst[cursor] = assignNthBitOfXtoY(byteToEmbed, dst[cursor], 8 - j - 1);

            cursor += hop;
            if (cursor == dstSize)
            {
                cursor = ++laps;
            }
            else if (cursor > dstSize)
            {
                cursor %= dstSize;
                laps++;
            }
        }
    }
}

uint8_t assignNthBitOfXtoY(uint8_t x, uint8_t y, int n)
{
    // Clear last bit of y
    y &= 0xFE;

    uint8_t bitToAssign = (x >> n) & 1;

    y |= bitToAssign;
    return y;
}

/* 
* REMEMBER THAT BMP FILES ARE READ FROM DOWNSIDE-UP AND FROM LEFT TO RIGHT
* so if [[0,2,3], [4,5,7]] is a bmp file, 
* then the order to read will be [4 -> 5 -> 7] -> [0 -> 2 -> 3] 
*/

/*
 * This function inserts into the least significant bits of the bmp file, all the
 * bits of the cipherText. It assumes all the validations have already been made.
 * 
 * @dev sizeof(uint8_t) = 1 byte (8 bits) 
 * @dev sizeof(int) = 4 bytes (32 bits) 
 * 
 * @param bmpFile: the bmpFile to do the stego-thing (a matrix of ints) 
 * @param cipherText: the cipherText to hide inside the bmpFile
 */
uint8_t *lsb1(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize, const size_t widthInBytes)
{
    printf("En lsb1 %d\n", cipherTextSize);
    size_t cCursor = 0;
    int cBitCursor = 7;
    uint8_t *stegoBmp = malloc(bmpFileSize);
    int bmpCursor = bmpFileSize - 1;
    size_t rowCursor = widthInBytes - 1;
    while (bmpCursor >= 0)
    {
        if (cBitCursor == -1)
        {
            cCursor++;
            cBitCursor = 7;
        }

        uint8_t newBmpByte;
        if (cCursor >= cipherTextSize)
        {
            newBmpByte = bmpFile[bmpCursor - rowCursor];
        }
        else
        {
            newBmpByte = replaceNthLSB(bmpFile[bmpCursor - rowCursor], cipherText[cCursor], cBitCursor--, 0);
        }

        stegoBmp[bmpCursor - rowCursor] = newBmpByte;

        if (rowCursor == 0)
        {
            rowCursor = widthInBytes - 1;
            bmpCursor -= widthInBytes;
        }
        else
        {
            rowCursor--;
        }
    }

    return stegoBmp;
}

// FIXME, this is kinda broken
// HANDLE CASE WHERE CIPHER TEXT IS BIGGER TAN BMP FILE --> I THINK THIS SHOULD BE IN ANOTHER PLACE
uint8_t *lsb4(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize, const size_t widthInBytes)
{
    printf("En lsb4 %d\n", cipherTextSize);
    size_t cCursor = 0;
    int cBitCursor = 7;
    uint8_t *stegoBmp = malloc(bmpFileSize);
    int bmpCursor = bmpFileSize - 1;
    size_t rowCursor = widthInBytes - 1;
    while (bmpCursor >= 0)
    {
        if (cBitCursor == -1)
        {
            cCursor++;
            cBitCursor = 7;
        }

        uint8_t newBmpByte;
        if (cCursor >= cipherTextSize)
        {
            newBmpByte = bmpFile[bmpCursor - rowCursor];
        }
        else
        {
            newBmpByte = replaceNthLSB(bmpFile[bmpCursor - rowCursor], cipherText[cCursor], cBitCursor--, 0);
            newBmpByte = replaceNthLSB(newBmpByte, cipherText[cCursor], cBitCursor--, 1);
            newBmpByte = replaceNthLSB(newBmpByte, cipherText[cCursor], cBitCursor--, 2);
            newBmpByte = replaceNthLSB(newBmpByte, cipherText[cCursor], cBitCursor--, 3);
        }

        stegoBmp[bmpCursor - rowCursor] = newBmpByte;

        if (rowCursor == 0)
        {
            rowCursor = widthInBytes - 1;
            bmpCursor -= widthInBytes;
        }
        else
        {
            rowCursor--;
        }
    }
    return stegoBmp;
}

uint8_t *lsbi(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize,
              const size_t cipherTextSize, const size_t hop, const size_t widthInBytes, const uint8_t *rc4Key)
{
    printf("En lsbi\n");
    size_t cCursor = 0;
    size_t cBitCursor = 7;
    uint8_t *stegoBmp = malloc(bmpFileSize + 6);
    int currentFirstIndexHop = 0;
    int rowCursor = widthInBytes - 1;
    int noMorehops = 0;
    int bmpCursor = bmpFileSize - 1;
    // // Copy rc4Key
    for (int i = 0; i <= 5; i++)
    {
        stegoBmp[bmpCursor - rowCursor] = rc4Key[i];
        rowCursor--;
    }

    while (currentFirstIndexHop < hop)
    {
        bmpCursor = bmpFileSize - 1;
        rowCursor = widthInBytes - 7 - currentFirstIndexHop;
        while (bmpCursor >= 0)
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

            if (cBitCursor == -1)
            {
                cCursor++;
                cBitCursor = 7;
            }

            uint8_t newBmpByte;
            if (cCursor >= cipherTextSize)
            {
                newBmpByte = bmpFile[bmpCursor - rowCursor];
            }
            else
            {
                newBmpByte = replaceNthLSB(bmpFile[bmpCursor - rowCursor], cipherText[cCursor], cBitCursor--, 0);
            }

            stegoBmp[bmpCursor - rowCursor] = newBmpByte;

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
        currentFirstIndexHop++;
    }
    rowCursor = widthInBytes - 1;
    return stegoBmp;
}
