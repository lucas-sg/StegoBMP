#include "../include/lsbHelper.h"


size_t extractSizeFromLSB1(const uint8_t *bmp);
size_t extractSizeFromLSB4(const uint8_t *bmp);
size_t extractSizeFromLSBI(const uint8_t *bmp, size_t bmpSize);
size_t getFirstBit(uint8_t byte);


size_t extractFourBytesOfSizeFrom(const uint8_t *bmp, STEGO_ALGO stegoAlgo, size_t bmpSize)
{
    if (stegoAlgo == LSB1)
        return extractSizeFromLSB1(bmp);
    else if (stegoAlgo == LSB4)
        return extractSizeFromLSB4(bmp);
    else
        return extractSizeFromLSBI(bmp, bmpSize);
}

size_t extractSizeFromLSB1(const uint8_t *bmp)
{
    uint32_t size = 0;

    for (int i = 0; i < SIZE_BYTES; i++)
    {
        uint32_t extractedByte = lsb1ExtractByte(i, bmp);
        size |= extractedByte << ((3u - i) * 8u);
    }

    return size;
}

uint8_t lsb1ExtractByte(size_t byteIndex, const uint8_t *bmp)
{
    uint8_t extractedByte = 0;

    for (uint8_t j = 0; j < 8; j++)
    {
        uint8_t extractedBit = bmp[byteIndex * 8 + j] & 1u;
        extractedByte |= (uint8_t) (extractedBit << (7u - j));
    }

    return extractedByte;
}

size_t extractSizeFromLSB4(const uint8_t *bmp)
{
    uint8_t *dst = malloc(4);

    for (int i = 0, j = 0; i < 8; i += 2, j++)
    {
        uint8_t extractedByte = lsb4ExtractByte(i, bmp);
        dst[j] = extractedByte;
    }

    return extractStegoSizeFrom(dst);
}

uint8_t lsb4ExtractByte(size_t byteIndex, const uint8_t *bmp)
{
    uint8_t extractedByte = 0;

    uint8_t firstFourBits  = bmp[byteIndex]   & 0x0Fu;
    uint8_t secondFourBits = bmp[byteIndex+1] & 0x0Fu;

    extractedByte |= (uint8_t) (firstFourBits << 4u);
    extractedByte |= secondFourBits;

    return extractedByte;
}

size_t extractSizeFromLSBI(const uint8_t *bmp, size_t bmpSize)
{
    size_t cursor = 0, laps = 0, hop = getHop(bmp[0]), resultingSize = 0;

    bmp += RC4_KEY_SIZE;

    for (size_t i = 0; i < 4 && cursor < bmpSize; i++)
    {
        uint8_t extractedByte = lsbiExtractByte(bmp, bmpSize, &cursor, &laps, hop);
        resultingSize |= (size_t) (extractedByte << ((3u - i) * 8u));
    }

    return resultingSize;
}

uint8_t lsbiExtractByte(const uint8_t *bmp, size_t bmpSize, size_t *cursor, size_t *laps, size_t hop)
{
    uint8_t extractedByte = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t extractedBit = bmp[*cursor] & 1u;
        extractedByte |= (uint8_t) (extractedBit << (7u - i));

        *cursor += hop;

        if (*cursor >= bmpSize)
        {
            *cursor = ++laps;
        }
    }

    return extractedByte;
}

// We assume always big endian
size_t extractStegoSizeFrom(const uint8_t *bytes)
{
    return (bytes[0] << 24u) | (bytes[1] << 16u) | (bytes[2] << 8u) | (bytes[3] << 0u);
}

size_t getHop(const uint8_t byte)
{
    if (byte == 0)
        return 256;
    else
        return getFirstBit(byte);
}

size_t getFirstBit(uint8_t byte)
{
    size_t i;

    for (i = 1; byte > 1 && i < 8; i++)
        byte = byte >> 1u;

    return byte << (i - 1);
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