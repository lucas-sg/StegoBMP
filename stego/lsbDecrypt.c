#include "lsbDecrypt.h"
#include "lsbEncrypt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t cipherTextByte, unsigned int cbCursor, unsigned int bitToReplace)

// We should change this function's name to lsb1Extraction or something like that
// Encrypt and decrypt seems unreasonable cause cipherText could be a non cipher one
uint8_t *lsb1Decrypt(const uint8_t *bmpFile, const size_t cipherTextSize, const size_t bmpSize)
{
    int bmpIndex = bmpSize - 1;
    int cipherTextIndex = 0;
    int cipherTextBitCursor = 7;
    uint8_t *cipherText = malloc(cipherTextSize);
    while (bmpIndex >= 0)
    {
        uint8_t auxByte;
        while (cipherTextBitCursor >= 0 && bmpIndex >= 0)
        {
            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpIndex--], 0);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, cipherTextBitCursor--);
        }
        cipherText[cipherTextIndex] = auxByte;
        cipherTextIndex++;
        cipherTextBitCursor = 7;
    }

    return cipherText;
}