#include "lsbExtract.h"
#include "lsbEmbed.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Keep in mind these functions could be abstracted a lot
 */

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpIndex = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    while (bmpIndex >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
        while (stegoBitCursor >= 0 && bmpIndex >= 0 && stegoCount >= 0)
        {
            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);
            rowCursor--;
            if (rowCursor == -1)
            {
                bmpIndex -= widthInBytes;
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
    printf("ENTRO\n");
    int bmpIndex = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    while (bmpIndex >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
        while (stegoBitCursor >= 0 && bmpIndex >= 0 && stegoCount >= 0)
        {
            printf("%d\n", bmpIndex - rowCursor);
            // FIRST LSB
            uint8_t firstBitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, firstBitToReplace, 0, stegoBitCursor--);
            // SECOND LSB
            uint8_t secondBitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 1);
            auxByte = replaceNthLSB(auxByte, secondBitToReplace, 0, stegoBitCursor--);
            // THIRD LSB
            uint8_t thirdBitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 2);
            auxByte = replaceNthLSB(auxByte, thirdBitToReplace, 0, stegoBitCursor--);
            // FOURTH LSB
            uint8_t fourthBitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 3);
            auxByte = replaceNthLSB(auxByte, fourthBitToReplace, 0, stegoBitCursor--);

            // printingBits(auxByte);

            rowCursor--;
            if (rowCursor == -1)
            {
                bmpIndex -= widthInBytes;
                rowCursor = widthInBytes - 1;
            }
        }
        stego[stegoIndex] = auxByte;

        stegoIndex++;
        stegoBitCursor = 7;
        stegoCount--;
    }

    stego[stegoSize] = 0;
    printf("RETORNA\n");
    return stego;
}
