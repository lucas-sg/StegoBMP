#include <stdio.h>
#include "../stego/lsbEncrypt.h"

unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
unsigned char *lsb4(const unsigned char *bmpFile, const char *cipherText);
unsigned char replaceNthLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned int cbCursor, unsigned int bitToReplace);
unsigned char flippingNthLSBToZero(const unsigned char bytes, int bitToReplace);
unsigned char getCurrentBitOfChar(const char cipherTextChar, unsigned int cbCursor);
int isCursorWithinOneByteRange(unsigned int cursor);

void byteCursorIsWithingRangeTest() {}
void byteCursorIsNotWithingRangeTest() {}
void getCurrentBitOfCharWithNullCipherTextAndInvalidCursorTest() {}
void getCurrentBitOfCharWithNullCipherTextAndValidCursorTest() {}
void getCurrentBitOfCharWithValidCipherTextAndInvalidCursorTest() {}
void getCurrentBitOfCharWithValidCipherTextAndValidCursorTest() {}
void flippingNthLSBToZeroWithNullBytesAndValidBitToReplaceTest() {}
void flippingNthLSBToZeroWithNullBytesAndInvalidBitToReplaceTest() {}
void flippingNthLSBToZeroWithValidBytesAndValidBitToReplaceTest() {}
void flippingNthLSBToZeroWithValidBytesAndInvalidBitToReplaceTest() {}
void replaceNthLSBWithInvalidCursorTest() {}
void replaceNthLSBWithInvalidBitToReplaceTest() {}
void replaceNthLSBWithValidConditionsTest() {}

int main()
{
    byteCursorIsWithingRangeTest();
    byteCursorIsNotWithingRangeTest();

    getCurrentBitOfCharWithNullCipherTextAndValidCursorTest();
    getCurrentBitOfCharWithNullCipherTextAndInvalidCursorTest();
    getCurrentBitOfCharWithValidCipherTextAndInvalidCursorTest();
    getCurrentBitOfCharWithValidCipherTextAndValidCursorTest();

    flippingNthLSBToZeroWithNullBytesAndValidBitToReplaceTest();
    flippingNthLSBToZeroWithNullBytesAndInvalidBitToReplaceTest();
    flippingNthLSBToZeroWithValidBytesAndValidBitToReplaceTest();
    flippingNthLSBToZeroWithValidBytesAndInvalidBitToReplaceTest();

    replaceNthLSBWithInvalidCursorTest();
    replaceNthLSBWithInvalidBitToReplaceTest();
    replaceNthLSBWithValidConditionsTest();
}