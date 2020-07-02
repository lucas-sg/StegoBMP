#include "../include/lsbEmbed.h"
#include "../include/lsbHelper.h"
#include <stdint.h>
#include <string.h>

uint8_t assignNthBitOfXtoY(uint8_t x, uint8_t y, int n);

void lsb1EmbedBytes(const uint8_t *src, uint8_t *dst, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        uint8_t byteToEmbed = src[i];

        // Needing 8 destination bytes per source byte.
        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t dstByte = dst[i * 8 + j];
            dst[i * 8 + j] = assignNthBitOfXtoY(byteToEmbed, dstByte, 7 - j);
        }
    }
}

void lsb4EmbedBytes(const uint8_t *src, uint8_t *dst, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        uint8_t byteToEmbed = src[i];

        uint8_t firstDstByte = dst[i * 2] & 0xF0;
        uint8_t secondDstByte = dst[i * 2 + 1] & 0xF0;

        firstDstByte |= (byteToEmbed & 0xF0) >> 4;
        secondDstByte |= byteToEmbed & 0x0F;

        dst[i * 2] = firstDstByte;
        dst[i * 2 + 1] = secondDstByte;
    }
}

/**
 * src: the thing to embed
 * dst: the file to hide the src
 * */
void lsbiEncryptAndEmbed(const uint8_t *src, uint32_t msgSize, uint8_t *dst, size_t dstSize)
{
    int hop = getHop(dst[0]);
    uint8_t *encSrc = RC4(src, dst, msgSize);
    lsbiEmbedBytes(encSrc, msgSize, dst, dstSize, hop);
}

void lsbiEmbedBytes(const uint8_t *src, size_t msgSize, uint8_t *dst, size_t dstSize, size_t hop)
{
    size_t laps = 0;
    uint8_t srcBitCursor = 0;
    size_t msgCount = 0;
    size_t bmpCursor = laps + RC4_KEY_SIZE;
    while (laps < hop && msgCount < msgSize)
    {
        size_t auxCursor = bmpCursor;
        if (auxCursor >= dstSize)
        {
            bmpCursor = ++laps + RC4_KEY_SIZE;
        }
        uint8_t byteToEmbed = src[msgCount];
        // Needing 8 destination bytes per source byte.
        for (; srcBitCursor < 8; srcBitCursor++)
        {
            auxCursor = bmpCursor + srcBitCursor * hop;
            if (auxCursor >= dstSize)
            {
                bmpCursor = ++laps + RC4_KEY_SIZE;
                break;
            }
            uint8_t dstByte = dst[auxCursor];
            dst[auxCursor] = assignNthBitOfXtoY(byteToEmbed, dstByte, 7 - srcBitCursor);
        }
        bmpCursor = auxCursor + hop;
        if (srcBitCursor == 8)
        {
            srcBitCursor = 0;
            msgCount++;
        }
    }
}

uint8_t assignNthBitOfXtoY(uint8_t x, uint8_t y, int n)
{
    // Clear last bit of y
    y &= 0xFE;

    uint8_t bitToAssign = (x >> n) & 1;

    y |= bitToAssign;
    return y;
}
