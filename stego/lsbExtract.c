#include "lsbExtract.h"
#include "lsbEmbed.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
int getHopFromBmpFile(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes);
unsigned int extractDecimalFromBinary(uint8_t binary);
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

// THIS FUNCTION SHOULD BE FIXED TO SUPPORT RC4 ENCRYPTION, SO KEY MUST BE EXTRACTED FROM LAST 6 BYTES OF BMP
uint8_t *lsbiExtract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    printf("En lsbi extract \n");
    int bmpIndex = bmpSize - 2;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    int currentHopIndex = 0;

    int hop = getHopFromBmpFile(bmpFile, stegoSize, widthInBytes);
    printf("El hop es %d\n", hop);
    printf("En get hop from bmp file \n");
    const uint8_t *rc4Key = extractRC4Key(bmpFile, stegoSize, widthInBytes);
    for (int i = 0; i < 5; i++)
        printingBits(rc4Key[i]);
    printf("\n");
    printf("En extrac rc4\n");
    uint8_t *stego = malloc(stegoSize);

    // while (currentHopIndex < hop)
    // {
    // }
    return stego;
}

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes)
{
    uint8_t *rc4Key = malloc(6 * sizeof(uint8_t));           // extract 6 to another constant
    size_t firstPixelIndex = stegoSize - 1 - (widthInBytes); // extract 3 to another constant
    memcpy(rc4Key, bmpFile - firstPixelIndex, 6);
    return rc4Key;
}

int getHopFromBmpFile(const uint8_t *bmpFile, const size_t stegoSize, const size_t widthInBytes)
{
    uint8_t hopByte = bmpFile[stegoSize - 1 - (widthInBytes)];
    if (hopByte == 0b00000000)
    {
        printf("Retorna 256\n");
        return 256;
    }
    printf("ExtractDecimalFromBinary %d\n", extractDecimalFromBinary(hopByte));
    return extractDecimalFromBinary(hopByte);
}

unsigned int extractDecimalFromBinary(uint8_t binary)
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

unsigned int pow(int num, int times)
{
    if (times == 0)
    {
        return num;
    }
    return num * pow(num, times - 1);
}