#include "lsbDecrypt.h"
#include "lsbEncrypt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t stegoFileByte, unsigned int cbCursor, unsigned int bitToReplace)

// We should change this function's name to lsb1Extraction or something like that
// Encrypt and decrypt seems unreasonable cause stegoFile could be a non cipher one
uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t stegoFileSize, const size_t bmpSize, const size_t widthInPixels)
{
    int bmpIndex = bmpSize - 1;
    int stegoFileIndex = 0;
    int stegoFileBitCursor = 7;
    uint8_t *stegoFile = malloc(stegoFileSize);
    size_t rowCursor = widthInPixels - 1;
    while (bmpIndex >= 0)
    {
        uint8_t auxByte;
        while (stegoFileBitCursor >= 0 && bmpIndex >= 0)
        {
            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpIndex - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoFileBitCursor--);
            rowCursor--;
            if (rowCursor == 0)
            {
                bmpIndex -= widthInPixels;
                rowCursor = widthInPixels - 1;
            }
        }
        stegoFile[stegoFileIndex] = auxByte;
        stegoFileIndex++;
        stegoFileBitCursor = 7;
    }

    return stegoFile;
}