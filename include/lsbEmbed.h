#ifndef LSBENCRYPT_H
#define LSBENCRYPT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lsbHelper.h"

uint8_t *lsb1(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize, const size_t widthInBytes);
uint8_t *lsb4(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize, const size_t widthInBytes);
uint8_t *lsbi(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize, const size_t hop,
              const size_t sizeInBytes, const uint8_t *rc4Key);
uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t cipherTextByte, unsigned int cBitCursor, unsigned int bitToReplace);
uint8_t flippingNthLSBToZero(const uint8_t bytes, int bitToReplace);
uint8_t getCurrentBitOf(const uint8_t cipherTextuint8_t, unsigned int cBitCursor);
int isCursorWithinOneByteRange(unsigned int cursor);
void printingBits(int number);

#endif