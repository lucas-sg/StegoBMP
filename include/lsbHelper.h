#ifndef LSBHELPER_H
#define LSBHELPER_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PIXEL_SIZE_IN_BYTES 3
#define SIZE_BYTES 4

int extractStegoSizeFrom(const uint8_t *bytes);
int isCursorWithinOneByteRange(unsigned int cursor);
uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t cipherTextByte, unsigned int cBitCursor, unsigned int bitToReplace);
void printingBits(int number);
uint8_t flippingNthLSBToZero(const uint8_t bytes, int bitToReplace);
uint8_t getCurrentBitOf(const uint8_t cipherTextuint8_t, unsigned int cBitCursor);
int getHopFromBmpFile(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes);

#endif