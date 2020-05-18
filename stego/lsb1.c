#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int *lsb1(const int *bmpFile, const char *cipherText);
int *replaceLeastSignificantBit(const int *bmpByte, const char cipherTextByte, int cbCursor);

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
int *lsb1(const int *bmpFile, const char *cipherText)
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
int *replaceLeastSignificantBit(const int *bmpByte, const char cipherTextByte, int cbCursor)
{
}


int main()
{
    printf("IN LSB1 MAIN \n");
    printf("sizeof(char) %ld \n", sizeof(char));
    printf("sizeof(int) %ld \n", sizeof(int));
}