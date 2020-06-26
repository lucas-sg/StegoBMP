#include "../include/lsbHelper.h"
#include "../include/types.h"

// We assume always big endian
unsigned int extractStegoSizeFrom(const uint8_t *bytes)
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3] << 0);
}

size_t extractFourBytesOfSizeFrom(const uint8_t *bytes, STEGO_ALGO stegoAlgo)
{
    if (stegoAlgo == LSB1)
    {
        return extractSizeFromLSB1(bytes);
    }
    return extractSizeFromLSB4(bytes);
}

size_t extractSizeFromLSB1(const uint8_t *bytes)
{
    uint8_t *dst = malloc(4);
    for (int i = 0; i < 4; i++)
    {
        uint8_t byte = 0;

        for (uint8_t j = 0; j < 7; j++)
        {
            uint8_t sourceByte = bytes[i * 8 + j] & 1;
            byte |= (sourceByte & 1) << (7 - j);
        }
        dst[i] = byte;
    }
    return extractStegoSizeFrom(dst);
}

size_t extractSizeFromLSB4(const uint8_t *bytes)
{
    uint8_t *dst = malloc(4);
    for (int i = 0; i < 4; i++)
    {
        uint8_t byte = 0;

        uint8_t firstSourceByte = bytes[i * 8] & 0x0F;
        uint8_t secondSourceByte = bytes[i * 8 + 1] & 0x0F;

        byte |= firstSourceByte << 4;
        byte |= secondSourceByte;

        dst[i] = byte;
    }
    return extractStegoSizeFrom(dst);
}

const uint8_t *sizeToByteArray(uint32_t size)
{
    uint8_t *bytes = malloc(4);
    uint32_t mask = 0xFF;

    bytes[0] = size;
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
    for (i = 1 << 7; i > 0; i >>= 1)
        printf("%u", !!(number & i));

    printf("\n");
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

int getHopFromBmpFile(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    uint8_t hopByte = bmpFile[bmpSize - 1 - widthInBytes - 1];
    if (hopByte == 0b00000000)
    {
        return 256;
    }
    return (int)hopByte;
}