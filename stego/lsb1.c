#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int *lsb1(const int *bmpFile, const char *cipherText);

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
 * @param bmpFile: the bmpFile to do the stego-thing 
 * @param cipherText: the cipherText to hide inside the bmpFile
 */ 
int *lsb1(const int *bmpFile, const char *cipherText)
{

}

int main()
{
    printf("IN LSB1 MAIN \n");
    printf("sizeof(char) %ld \n", sizeof(char));
    printf("sizeof(int) %ld \n", sizeof(int));

}