#include "lsbEncrypt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
uint8_t *lsb4(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize)
{
    size_t cCursor = 0;
    size_t cBitCursor = 0;
    uint8_t *stegoBmp = malloc(bmpFileSize);
    for (size_t bmpCursor = 0; bmpCursor < bmpFileSize - 1;)
    {
        if (cBitCursor == 8)
        {
            cCursor++;
            cBitCursor = 0;
        }

        // handle error here, bmpFile's length could be lower than cipherText's length and viceversa
        uint8_t lsb1 = replaceNthLSB(bmpFile[bmpCursor], cipherText[cCursor++], cBitCursor, 0);
        uint8_t lsb2 = replaceNthLSB(lsb1, cipherText[cCursor++], cBitCursor, 1);
        uint8_t lsb3 = replaceNthLSB(lsb2, cipherText[cCursor++], cBitCursor, 2);
        uint8_t lsb4 = replaceNthLSB(lsb3, cipherText[cCursor++], cBitCursor, 3);

        stegoBmp[bmpCursor] = lsb4;

        bmpCursor += 4;
    }
    stegoBmp[bmpFileSize + 1] = '\0';

    return stegoBmp;
}

int isCursorWithinOneByteRange(unsigned int cursor)
{
    return cursor >= 0 && cursor <= 7;
}

/* 
 * This function replaces the least significant bit of the current bmp files' byte
 * with the current position of the bit in the current uint8_tacter from the ciphertext
 *
 * @param bmpByte: the current bmp byte (pixel) to replace the LSB
 * @param cipherTextByte: the current ciphertext byte 
 * @param cBitCursor: represents the cipher text byte cursor (0 <= cBitCursor <= 7)
 */
uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t cipherTextByte, unsigned int cBitCursor, unsigned int bitToReplace)
{
    // flipping nth bit of bmpByte to 0
    uint8_t bmpWithLSBToZero = flippingNthLSBToZero(bmpByte, bitToReplace);
    // changing nth bit of bmpByte to cipherTextByte[cBitCursor]
    uint8_t newBmpByte = (getCurrentBitOf(cipherTextByte, cBitCursor) << bitToReplace) | bmpWithLSBToZero;

    return newBmpByte;
}

// TODO remove this, for test purposes
void printingBits(int number)
{
    unsigned i;
    // Reverse loop
    // printf("%u", !!(number & (1 << 0)));

    for (i = 1 << 7; i > 0; i >>= 1)
        printf("%u", !!(number & i));

    // printf("\n");
}

uint8_t flippingNthLSBToZero(const uint8_t bytes, int bitToReplace)
{
    return bytes & (~(1 << bitToReplace));
}

/**
 * Bits are count starting from LSB
 */
uint8_t getCurrentBitOf(const uint8_t cipherTextuint8_t, unsigned int cBitCursor)
{
    return ((cipherTextuint8_t >> cBitCursor) & 1);
}
