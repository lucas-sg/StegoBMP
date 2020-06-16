#ifndef LSB1_H
#define LSB1_H

#include <stdio.h>
#include <assert.h>

unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
unsigned char *lsb4(const unsigned char *bmpFile, const char *cipherText);
unsigned char replaceNthLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned int cbCursor, unsigned int bitToReplace);
unsigned char flippingNthLSBToZero(const unsigned char bytes, int bitToReplace);
unsigned char getCurrentBitOfChar(const char cipherTextChar, unsigned int cbCursor);
int isCursorWithinOneByteRange(unsigned int cursor);

#endif