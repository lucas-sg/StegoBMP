#ifndef LSB_HELPER_H
#define LSB_HELPER_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"

#define SIZE_BYTES 4
#define RC4_KEY_SIZE 6
// It is assumed that we will not be dealing with file extensions larger than 7 chars + the leading period
#define MAX_EXTENSION_SIZE 8

int isCursorWithinOneByteRange(unsigned int cursor);
uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t cipherTextByte, unsigned int cBitCursor, unsigned int bitToReplace);
void printingBits(int number);
uint8_t flippingNthLSBToZero(const uint8_t bytes, int bitToReplace);
uint8_t getCurrentBitOf(const uint8_t cipherText, unsigned int cBitCursor);
size_t extractFourBytesOfSizeFrom(const uint8_t *bytes, STEGO_ALGO stegoAlgo, size_t bmpSize);
size_t extractSizeFromLSB1(const uint8_t *bmp);
uint8_t lsb1ExtractByte(size_t byteIndex, const uint8_t *bmp);
uint8_t lsb4ExtractByte(size_t byteIndex, const uint8_t *bmp);
uint8_t lsbiExtractByte(const uint8_t *bmp, size_t bmpSize, size_t *cursor, size_t *laps, size_t hop);
size_t getHop(const uint8_t bmp);
uint32_t getSizeFromPointer(const uint8_t *pointer);
void copyFileExtension(uint8_t *dst, const uint8_t *src);

#endif