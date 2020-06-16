#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
unsigned char *lsb4(const unsigned char *bmpFile, const char *cipherText);
unsigned char replaceNthLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned int cbCursor, unsigned int bitToReplace);
unsigned char flippingNthLSBToZero(const unsigned char bytes, int bitToReplace);
unsigned char getCurrentBitOfChar(const char cipherTextChar, unsigned int cbCursor);
int isCursorWithinOneByteRange(unsigned int cursor);
size_t strlen(const char *s);

/* 
* REMEMBER THAT BMP FILES ARE READ FROM DOWNSIDE-UP AND FROM LEFT TO RIGHT
* so if [[0,2,3], [4,5,7]] is a bmp file, 
* then the order to read will be [4 -> 5 -> 7] -> [0 -> 2 -> 3] 
*/

/*
 * This function inserts into the least significant bits of the bmp file, all the
 * bits of the cipherText. It assumes all the validations have already been made.
 * 
 * @dev sizeof(u char) = 1 byte (8 bits) 
 * @dev sizeof(int) = 4 bytes (32 bits) 
 * @dev sizeof(pixel) = 1 byte (8 bits)
 * 
 * @param bmpFile: the bmpFile to do the stego-thing (a matrix of ints) 
 * @param cipherText: the cipherText to hide inside the bmpFile
 */
unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText)
{
    size_t bmpFileSize = strlen(bmpFile);
    size_t cCursor = 0;
    size_t cBitCursor = 0;
    unsigned char *stegoBmp = malloc(bmpFileSize + 1);
    for (size_t bmpCursor = 0; bmpCursor < strlen(bmpFile);)
    {
        if (cBitCursor == 8)
        {
            cCursor++;
            cBitCursor = 0;
        }

        unsigned char newBmpByte = replaceNthLSB(bmpFile[bmpCursor++], cipherText[cCursor++], cBitCursor, 0);

        if (newBmpByte == NULL)
        {
            return NULL; // handle this
        }

        stegoBmp[bmpCursor] = newBmpByte;
    }
    stegoBmp[bmpFileSize + 1] = '\0';

    return stegoBmp;
}

// HANDLE CASE WHERE CIPHER TEXT IS BIGGER TAN BMP FILE --> I THINK THIS SHOULD BE IN ANOTHER PLACE
unsigned char *lsb4(const unsigned char *bmpFile, const char *cipherText)
{
    size_t bmpFileSize = strlen(bmpFile);
    size_t cCursor = 0;
    size_t cBitCursor = 0;
    unsigned char *stegoBmp = malloc(bmpFileSize + 1);
    for (size_t bmpCursor = 0; bmpCursor < strlen(bmpFile);)
    {
        if (cBitCursor == 8)
        {
            cCursor++;
            cBitCursor = 0;
        }

        // handle error here, bmpFile's length could be lower than cipherText's length and viceversa
        unsigned char lsb1 = replaceNthLSB(bmpFile[bmpCursor], cipherText[cCursor++], cBitCursor, 0);
        unsigned char lsb2 = replaceNthLSB(lsb1, cipherText[cCursor++], cBitCursor, 1);
        unsigned char lsb3 = replaceNthLSB(lsb2, cipherText[cCursor++], cBitCursor, 2);
        unsigned char lsb4 = replaceNthLSB(lsb3, cipherText[cCursor++], cBitCursor, 3);

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
 * with the current position of the bit in the current character from the ciphertext
 *
 * @param bmpByte: the current bmp byte (pixel) to replace the LSB
 * @param cipherTextByte: the current ciphertext byte 
 * @param cbCursor: represents the cipher text byte cursor (0 <= cbCursor <= 7)
 */
unsigned char replaceNthLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned int cbCursor, unsigned int bitToReplace)
{
    printf("Printing cipherText bits\n");
    printingBits(cipherTextByte);
    printf("Printing bmp bits\n");
    printingBits(bmpByte);

    // flipping nth bit of bmpByte to 0
    unsigned char bmpWithLSBToZero = flippingNthLSBToZero(bmpByte, bitToReplace);
    // changing nth bit of bmpByte to cipherTextByte[cbCursor]
    unsigned char newBmpByte = (getCurrentBitOfChar(cipherTextByte, cbCursor) << bitToReplace) | bmpWithLSBToZero;
    printf("printing new bmp bits\n");
    printingBits(newBmpByte);
    printf("_______________________\n");

    return newBmpByte;
}

// TODO remove this
void printingBits(int number)
{
    unsigned i;
    // Reverse loop
    for (i = 1 << 7; i > 0; i >>= 1)
        printf("%u", !!(number & i));
    printf("\n");
}

unsigned char flippingNthLSBToZero(const unsigned char bytes, int bitToReplace)
{
    return bytes & (~(1 << bitToReplace));
}

/**
 * Bits are count starting from LSB
 */
unsigned char getCurrentBitOfChar(const char cipherTextChar, unsigned int cbCursor)
{
    return ((cipherTextChar >> cbCursor) & 1);
}
