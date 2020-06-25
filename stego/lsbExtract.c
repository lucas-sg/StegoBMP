#include "../include/lsbExtract.h"
#include "../include/lsbEmbed.h"
#include "../include/lsbHelper.h"
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

// Extract N bytes from dst
void lsb1ExtractBytes(const uint8_t* src, uint8_t* dst, size_t size) {
    for (int i = 0; i < size; i++) {
        uint8_t byte = 0;

        for (uint8_t j = 0; j < 7; j++) {
            uint8_t sourceByte = src[i*8 + j] & 1;
            byte |= (sourceByte & 1) << (7 - j);
        }
        dst[i] = byte;
    }
}

void lsb4ExtractBytes(const uint8_t* source, uint8_t* dst, size_t N) {
    for (int i = 0; i < N; i++) {
        uint8_t byte = 0;

        uint8_t firstSourceByte = source[i*8] & 0x0F;
        uint8_t secondSourceByte = source[i*8 + 1] & 0x0F;

        byte |= firstSourceByte << 4;
        byte |= secondSourceByte;

        dst[i] = byte;
    }
}

uint32_t lsbiExtractSize(const uint8_t* source, size_t sourceSize, int hop, int* cursor, int* laps) {
    int auxCursor = *cursor, auxLaps = *laps;
    uint8_t msgBytes[4];
    for (int i = 0; i < 4; i++) {
        uint8_t byte = 0;
        // Needing 8 destination bytes per source byte.
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t sourceByte = source[auxCursor] & 1;
            byte |= (sourceByte & 1) << (7 - j);

            auxCursor += hop;
            if (auxCursor == sourceSize) {
                auxCursor = ++auxLaps;
            } else if(auxCursor > sourceSize) {
                auxCursor %= sourceSize;
                auxLaps++;
            }
        }
        msgBytes[i] = byte;
    }
    *cursor = auxCursor;
    *laps = auxLaps;

    return extractStegoSizeFrom(msgBytes);
}

size_t lsbiExtractBytes(const uint8_t* source, size_t sourceSize, uint8_t* dst, int hop) {
    int cursor = 0;
    int laps = 0;

    uint32_t msgSize = lsbiExtractSize(source, sourceSize, hop, &cursor, &laps);

    for (int i = 0; i < msgSize; i++) {
        uint8_t byte = 0;

        for (uint8_t j = 0; j < 8; j++) {
            uint8_t sourceByte = source[cursor] & 1;
            byte |= (sourceByte & 1) << (7 - j);

            cursor += hop;
            if (cursor == sourceSize) {
                cursor = ++laps;
            } else if(cursor > sourceSize) {
                cursor %= sourceSize;
                laps++;
            }
        }
        dst[i] = byte;
    }
    return msgSize;
}

void lsbiExtractAndDecrypt(const uint8_t* source, size_t sourceSize, uint8_t* dst) {
    int hop = source[0] == 0?255:source[0];
    uint8_t* encMsg = malloc(sourceSize);
    size_t msgSize = lsbiExtractBytes(source + 6, sourceSize - 6, encMsg, hop);
    uint8_t* decrypted = RC4decrypt(encMsg, source, msgSize);
    memcpy(dst, decrypted, msgSize);
}

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes)
{
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    while (bmpCursor >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
        while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
        {
            uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);
            rowCursor--;
            if (rowCursor == -1)
            {
                bmpCursor -= widthInBytes;
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
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    uint8_t *stego = malloc(stegoSize + 1);
    int rowCursor = widthInBytes - 1;
    int stegoCount = stegoSize - 1;
    while (bmpCursor >= 0 && stegoCount >= 0)
    {
        uint8_t auxByte;
        while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
        {
            // FIRST LSB
            uint8_t firstBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 0);
            auxByte = replaceNthLSB(auxByte, firstBitToReplace, 0, stegoBitCursor--);
            // SECOND LSB
            uint8_t secondBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 1);
            auxByte = replaceNthLSB(auxByte, secondBitToReplace, 0, stegoBitCursor--);
            // THIRD LSB
            uint8_t thirdBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 2);
            auxByte = replaceNthLSB(auxByte, thirdBitToReplace, 0, stegoBitCursor--);
            // FOURTH LSB
            uint8_t fourthBitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 3);
            auxByte = replaceNthLSB(auxByte, fourthBitToReplace, 0, stegoBitCursor--);

            rowCursor--;
            if (rowCursor == -1)
            {
                bmpCursor -= widthInBytes;
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
    int bmpCursor = bmpSize - 1;
    int stegoIndex = 0;
    int stegoBitCursor = 7;
    int currentHopIndex = 0;
    int rowCursor = widthInBytes - 1;
    int hop = 256;
    const uint8_t *rc4Key = extractRC4Key(bmpFile, bmpSize, widthInBytes);
    uint8_t *stego = malloc(stegoSize + 1);
    int stegoCount = stegoSize - 1;

    while (currentHopIndex < hop && stegoCount >= 0)
    {
        bmpCursor = bmpSize - 1;
        rowCursor = widthInBytes - 7 - currentHopIndex;
        while (bmpCursor >= 0 && stegoCount >= 0)
        {
            uint8_t auxByte;
            while (stegoBitCursor >= 0 && bmpCursor >= 0 && stegoCount >= 0)
            {
                if (rowCursor < 0)
                {
                    int diff = hop + rowCursor;
                    rowCursor = widthInBytes - 1 - diff;
                    bmpCursor -= widthInBytes;

                    if (bmpCursor < 0)
                    {
                        break;
                    }
                }
                uint8_t bitToReplace = getCurrentBitOf(bmpFile[bmpCursor - rowCursor], 0);
                auxByte = replaceNthLSB(auxByte, bitToReplace, 0, stegoBitCursor--);

                if (rowCursor <= 0)
                {
                    rowCursor = widthInBytes - 1;
                    bmpCursor -= widthInBytes;
                }
                else
                {
                    rowCursor -= hop;
                }
            }
            stego[stegoIndex++] = auxByte;
            stegoBitCursor = 7;
            stegoCount--;
        }
        currentHopIndex++;
    }
    stego[stegoSize] = 0;

    return stego;
}

uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes)
{
    uint8_t *rc4Key = malloc(6 * sizeof(uint8_t));           // extract 6 to another constant
    size_t firstPixelIndex = bmpSize - 1 - widthInBytes - 1; // extract 3 to another constant
    memcpy(rc4Key, bmpFile - firstPixelIndex, 6);
    return rc4Key;
}

//FIXME THIS IS BROKEN
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