#include "lsbDecrypt.h"
#include "lsbEncrypt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t stegoByte, unsigned int cbCursor, unsigned int bitToReplace)

// We should change this function's name to lsb1Extraction or something like that
// Encrypt and decrypt seems unreasonable cause stego could be a non cipher one
uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpIndex = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    int i = 0;
    while (bmpIndex >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
        while (stegoBitCursor >= 0 && bmpIndex >= 0 && stegoCount >= 0)
        {
            // printf("%d\n", bmpIndex - rowCursor);
            // printingBits(bmpFile[bmpIndex - rowCursor]);

            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 0);
            printf("%d", bitToReplace & 0x1);
            // printingBits(bitToReplace);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);
            i++;
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