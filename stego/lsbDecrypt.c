#include "lsbDecrypt.h"
#include "lsbEncrypt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/** FIXME
 * In a first iteration we'll asume that the stego size is not extracted from bmpFile and the size is known
 * Then we'll extract it from bmpFile and we could start de bmpIndex at bmpSize - 5 
 * since that sizeof(stegoSizeInfo) = 4bytes
 */

// We should change this function's name to lsb1Extraction or something like that
// Encrypt and decrypt seems unreasonable cause stego could be a non cipher one
uint8_t *lsb1Decrypt(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize)
{
    int bmpIndex = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize);
    while (bmpIndex >= 0)
    {
        uint8_t auxByte;
        while (stegoBitCursor >= 0 && bmpIndex >= 0)
        {
            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpIndex--], 0);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);
        }
        stego[stegoIndex] = auxByte;
        stegoIndex++;
        stegoBitCursor = 7;
    }

    return stego;
}

// THIS FUNCTION SHOULD BE FIXED TO SUPPORT RC4 ENCRYPTION, SO KEY MUST BE EXTRACTED FROM LAST 6 BYTES OF BMP
uint8_t *lsbiDecryption(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInPixels)
{
    uint bmpIndex = bmpSize - 2;
    uint stegoIndex = 0;
    uint stegoBitCursor = 7;
    uint currentHopIndex = 0;
    uint hop = getHopFromBmpFile(bmpFile, stegoSize);
    const uint8_t *rc4Key = extractRC4Key(bmpFile, stegoSize);
    uint8_t *stego = malloc(stegoSize);

    while (currentHopIndex < hop)
    {
    }
    return stego;
}

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInPixels)
{
    uint8_t *rc4Key = malloc(6 * sizeof(uint8_t));                // extract 6 to another constant
    size_t firstPixelIndex = stegoSize - 1 - (widthInPixels * 3); // extract 3 to another constant
    memcpy(rc4Key, bmpFile - firstPixelIndex, 6);
    return rc4Key;
}

uint getHopFromBmpFile(const uint8_t *bmpFile, const size_t stegoSize)
{
    uint8_t hopByte = bmpFile[stegoSize - 1];
    if (hopByte == 0b00000000)
    {
        return 256;
    }
    return extractDecimalFromBinary(hopByte);
}

uint extractDecimalFromBinary(uint8_t binary)
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