#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lsbExtract.h"

/**
 * Keep in mind these functions could be abstracted a lot
 */

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    printf("EN LSB1 EXTRACT\n");
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    int stegoSize = -1;
    int rowCursor = widthInBytes - 1;
    int stegoCount = 4;
    uint8_t *stego = malloc(5);
    uint8_t auxByte;

    while (bmpCursor >= 0 && stegoCount >= 0)
    {
        if (stegoCount == 0 && stegoSize == -1)
        {
            stegoSize = extractStegoSizeFrom(stego);
            stegoCount = stegoSize - 5;
            stego = realloc(stego, stegoSize);
        }

        while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
        {
            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);
            rowCursor--;
            if (rowCursor == -1)
            {
                bmpCursor -= widthInBytes;
                rowCursor = widthInBytes - 1;
            }
        }
        stego[stegoIndex] = auxByte;

        stegoIndex++;
        stegoBitCursor = 7;
        stegoCount--;
    }

    stego[stegoSize] = 0;

    return stego;
}

uint8_t *lsb4Extract(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    int stegoSize = -1;
    int rowCursor = widthInBytes - 1;
    int stegoCount = 4;
    uint8_t auxByte;
    uint8_t *stego = malloc(5);

    while (bmpCursor >= 0 && stegoCount >= 0)
    {
        if (stegoCount == 0 && stegoSize == -1)
        {
            stegoSize = extractStegoSizeFrom(stego);
            stegoCount = stegoSize - 5;
            stego = realloc(stego, stegoSize);
        }
        while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
        {
            // FIRST LSB
            uint8_t firstBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, firstBitToReplace, 0, stegoBitCursor--);
            // SECOND LSB
            uint8_t secondBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 1);
            auxByte = replaceNthLSB(auxByte, secondBitToReplace, 0, stegoBitCursor--);
            // THIRD LSB
            uint8_t thirdBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 2);
            auxByte = replaceNthLSB(auxByte, thirdBitToReplace, 0, stegoBitCursor--);
            // FOURTH LSB
            uint8_t fourthBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 3);
            auxByte = replaceNthLSB(auxByte, fourthBitToReplace, 0, stegoBitCursor--);

            rowCursor--;
            if (rowCursor == -1)
            {
                bmpCursor -= widthInBytes;
                rowCursor = widthInBytes - 1;
            }
        }
        stego[stegoIndex] = auxByte;

        stegoIndex++;
        stegoBitCursor = 7;
        stegoCount--;
    }

    stego[stegoSize] = 0;
    return stego;
}

// THIS FUNCTION SHOULD BE FIXED TO SUPPORT RC4 ENCRYPTION, SO KEY MUST BE EXTRACTED FROM LAST 6 BYTES OF BMP
uint8_t *lsbiExtract(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoSize = -1;
    int stegoBitCursor = 7;
    int currentHopIndex = 0;
    int rowCursor = widthInBytes - 1;
    int hop = getHopFromBmpFile(bmpFile, bmpSize, widthInBytes);
    printf("HOP %d\n", hop);
    int stegoCount = stegoSize - 1;
    const uint8_t *rc4Key = extractRC4Key(bmpFile, bmpSize, widthInBytes);
    uint8_t *stego = malloc(4);
    uint8_t auxByte;

    while (currentHopIndex < hop && stegoCount >= 0)
    {
        bmpCursor = bmpSize - 1;
        rowCursor = widthInBytes - 7 - currentHopIndex;
        while (bmpCursor >= 0 && stegoCount >= 0)
        {
            if (stegoCount == 0 && stegoSize == -1)
            {
                stegoSize = extractStegoSizeFrom(stego);
                stegoCount = stegoSize - 5;
                stego = realloc(stego, stegoSize);
            }
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
