#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
unsigned char replaceLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned char cbCursor);
unsigned char flippingLSBToZero(const unsigned char bytes);
unsigned char getLastBitOfChar(const char cipherTextChar, unsigned char cbCursor);

/* 
* REMEMBER THAT BMP FILES ARE READ FROM DOWNSIDE-UP AND FROM LEFT TO RIGHT
* so if [[0,2,3], [4,5,7]] is a bmp file, 
* then the order to read will be [4 -> 5 -> 7] -> [0 -> 2 -> 3] 
*/

/*
 * This function inserts into the least significant bits of the bmp file, all the
 * bits of the cipherText.
 * 
 * @dev sizeof(char) = 1 byte (8 bits) 
 * @dev sizeof(int) = 4 bytes (32 bits) 
 * @dev sizeof(pixel) = 1 byte (8 bits)
 * 
 * @param bmpFile: the bmpFile to do the stego-thing (a matrix of ints) 
 * @param cipherText: the cipherText to hide inside the bmpFile
 */
unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText)
{
}

/* 
 * This function replaces the least significant bit of the current bmp files' byte
 * with the current position of the bit in the current character from the ciphertext
 *
 * @param bmpByte: the current bmp byte (pixel) to replace the LSB
 * @param cipherTextByte: the current ciphertext byte 
 * @param cbCursor: represents the cipher text byte cursor (0 <= cbCursor <= 7)
 */
unsigned char replaceLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned char cbCursor)
{
    printf("Printing cipherText bits\n");
    printingBits(cipherTextByte);
    printf("Printing bmp bits\n");
    printingBits(bmpByte);

    // flipping last bit of bmpByte to 0
    unsigned char bmpWithLSBToZero = flippingLSBToZero(bmpByte);
    // changing last bit of bmpByte to cipherTextByte[cbCursor]
    unsigned char newBmpByte = (bmpByte & ~1) | getLastBitOfChar(cipherTextByte, cbCursor);
    
    printf("printing new bmp bits\n");
    printingBits(newBmpByte);
    
    return newBmpByte;
}

// TODO remove this
void printingBits(int number)
{
    unsigned i;
    // Reverse loop
    for (i = 1 << 31; i > 0; i >>= 1)
        printf("%u", !!(number & i));
    printf("\n");
}

unsigned char flippingLSBToZero(const unsigned char bytes) {
    return (bytes & ~1) | 0;
}

unsigned char getLastBitOfChar(const char cipherTextChar, unsigned char cbCursor) {
    return ((cipherTextChar >> cbCursor) & 1);
}

unsigned int main()
{
    printf("IN LSB1 MAIN \n");
    replaceLSB(5, 'p', 0);
}