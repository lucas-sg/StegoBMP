#include "lsbExtract.h"
#include "lsbEmbed.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
int getHopFromBmpFile(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
unsigned int extractDecimalFromBinary(uint8_t binary);
/**
 * Keep in mind these functions could be abstracted a lot
 */

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    while (bmpCursor >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
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

uint8_t *lsb4Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    while (bmpCursor >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
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
uint8_t *lsbiExtract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    printf("En lsbi extract, stegoSize %d \n", stegoSize);
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    int currentHopIndex = 0;
    int rowCursor = widthInBytes - 1;
    int hop =256;
    const uint8_t *rc4Key = extractRC4Key(bmpFile, bmpSize, widthInBytes);
    uint8_t *stego = malloc(stegoSize + 1);
    int stegoCount = stegoSize - 1;

    while (currentHopIndex < hop && stegoCount >= 0)
    {
        bmpCursor = bmpSize - 1;
        rowCursor = widthInBytes - 7 - currentHopIndex;
        // printf("%d\n", currentHopIndex);
        while (bmpCursor >= 0 && stegoCount >= 0)
        {
            uint8_t auxByte;
            while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
            {
                // printf("Stego count %d\n", stegoCount);
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
                // printf("%d", bmpCursor - rowCursor);
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
            // printf("%c", auxByte);
            stego[stegoIndex++] = auxByte;
            stegoBitCursor = 7;
            stegoCount--;
        }
        currentHopIndex++;
    }
    stego[stegoSize] = 0;

    printf("AWDASDASD\n");
    for (int i = 0; i < 102; i++)
        printf("%c", stego[i]);
    printf("\n");
    return stego;
}

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    uint8_t *rc4Key = malloc(6 * sizeof(uint8_t));           // extract 6 to another constant
    size_t firstPixelIndex = bmpSize - 1 - widthInBytes - 1; // extract 3 to another constant
    memcpy(rc4Key, bmpFile - firstPixelIndex, 6);
    return rc4Key;
}

int getHopFromBmpFile(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    uint8_t hopByte = bmpFile[bmpSize - 1 - widthInBytes - 1];
    printingBits(hopByte);
    if (hopByte == 0b00000000)
    {
        return 256;
    }
    return extractDecimalFromBinary(hopByte);
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

unsigned int pow(int num, int times)
{
    if (times == 0)
    {
        return num;
    }
    return num * pow(num, times - 1);
}