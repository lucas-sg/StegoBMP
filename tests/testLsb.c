#include "../stego/lsbEncrypt.h"

unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
unsigned char *lsb4(const unsigned char *bmpFile, const char *cipherText);
unsigned char replaceNthLSB(const unsigned char bmpByte, const char cipherTextByte, unsigned int cbCursor, unsigned int bitToReplace);
unsigned char flippingNthLSBToZero(const unsigned char bytes, int bitToReplace);
unsigned char getCurrentBitOfChar(const char cipherTextChar, unsigned int cbCursor);
int isCursorWithinOneByteRange(unsigned int cursor);

static char *TEST_PASSED = "Test passed!!\n";
static char *RUNNING_TEST = "Running test: %s\n";

void byteCursorIsWithingRangeTest()
{
    printf(RUNNING_TEST, __func__);

    unsigned int cursor = 8;

    assert(isCursorWithinOneByteRange(cursor) == 0);

    printf(TEST_PASSED);
    printf("Cursor %d is whithin the valid range of a byte cursor\n\n", cursor);
}

void byteCursorIsNotWithingRangeTest()
{
    printf(RUNNING_TEST, __func__);

    unsigned int cursor = 23;

    assert(isCursorWithinOneByteRange(cursor) == 0);

    printf(TEST_PASSED);
    printf("Cursor %d is not whithin the valid range of a byte cursor\n\n", cursor);
}

// unsigned char getCurrentBitOfChar(const char cipherTextChar, unsigned int cbCursor);
void getCurrentBitOfCharWithValidCipherTextAndValidCursorTest()
{
    printf(RUNNING_TEST, __func__);

    unsigned int cbCursor = 0;
    const char cipherTextByte = 0b0100001;

    assert(getCurrentBitOfChar(cipherTextByte, cbCursor) == 0b00000001);

    printf(TEST_PASSED);
    printf("First LSB from cipherTextByte 0b0100001 is 1\n\n");
}

void getCurrentBitOfCharWithValidCipherTextAndValidCursorTest2()
{

    printf(RUNNING_TEST, __func__);

    unsigned int cbCursor = 1;
    const char cipherTextByte = 0b0100001;

    assert(getCurrentBitOfChar(cipherTextByte, cbCursor) == 0b00000000);

    printf(TEST_PASSED);
    printf("First LSB from cipherTextByte 0b0100001 is 0\n\n");
}

// unsigned char flippingNthLSBToZero(const unsigned char bytes, int bitToReplace);
void flipFirstLSBOfAZeroBitToZeroTest()
{
    printf(RUNNING_TEST, __func__);

    unsigned int bitToReplace = 0;
    const char byte = 0b00001111;

    assert(flippingNthLSBToZero(byte, bitToReplace) == 0b00001110);

    printf(TEST_PASSED);
    printf("Last bit of 0b00001111 is now 0: 0b00001110\n\n");
}

void flipFirstLSBOfAOneBitToZeroTest()
{
    printf(RUNNING_TEST, __func__);

    unsigned int bitToReplace = 2;
    const char byte = 0b00001111;

    assert(flippingNthLSBToZero(byte, bitToReplace) == 0b00001011);

    printf(TEST_PASSED);
    printf("Last bit of 0b00001111 is now 0: 0b00001011\n\n");
}

// unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
void lsb1WithAllOnesAndSomeBytesWithoutStegoTest()
{
    printf(RUNNING_TEST, __func__);
    const char bmpFile[9] = {0b00011110, 0b11111110, 0b00011000,
                             0b00011000, 0b01010010, 0b11011110,
                             0b11011110, 0b01011110, 0b00011110};
    const char cipherText[1] = {0b11111111};

    const char *newBmpFile = lsb1(bmpFile, cipherText);
    for (int i = 8; i >= 0; i--)
    {
        if (i <= 8 && i >= 1)
        {
            assert(getCurrentBitOfChar(newBmpFile[i], 0) == getCurrentBitOfChar(cipherText[0], 8 - i));
        }
        else
        {
            assert(getCurrentBitOfChar(newBmpFile[i], 0) == getCurrentBitOfChar(bmpFile[i], 0));
        }
    }
    printf(TEST_PASSED);
    printf("LSB1 on current bmp file with c = 1^8 has worked as expected\n\n");
}

void lsb1WithZerosAndOnesWithAllBytesInStegoTest()
{
    printf(RUNNING_TEST, __func__);
    const char bmpFile[9] = {0b00011111, 0b11111110, 0b00011001,
                             0b00011000, 0b01010011, 0b11011110,
                             0b11011111, 0b01011110};
    const char cipherText[1] = {0b01010101};

    const char *newBmpFile = lsb1(bmpFile, cipherText);
    for (int i = 7; i >= 0; i--)
    {
        assert(getCurrentBitOfChar(newBmpFile[i], 0) == getCurrentBitOfChar(cipherText[0], 7 - i));
    }
    printf(TEST_PASSED);
    printf("LSB1 on current bmp file with c = (01)^4 has worked as expected\n\n");
}

// unsigned char *lsb1(const unsigned char *bmpFile, const char *cipherText);
void lsb1WithAllZerosAndSomeBytesWithoutStegoTest()
{
    printf(RUNNING_TEST, __func__);
    const char bmpFile[9] = {0b00011111, 0b11111111, 0b00011001,
                             0b00011001, 0b01010011, 0b11011111,
                             0b11011111, 0b01011111, 0b00011110};
    const char cipherText[1] = {0b00000000};

    const char *newBmpFile = lsb1(bmpFile, cipherText);
    for (int i = 8; i >= 0; i--)
    {
        if (i <= 8 && i >= 1)
        {
            assert(getCurrentBitOfChar(newBmpFile[i], 0) == getCurrentBitOfChar(cipherText[0], 8 - i));
        }
        else
        {
            assert(getCurrentBitOfChar(newBmpFile[i], 0) == getCurrentBitOfChar(bmpFile[i], 0));
        }
    }
    printf(TEST_PASSED);
    printf("LSB1 on current bmp file with c=0^8 has worked as expected\n\n");
}

int main()
{
    byteCursorIsWithingRangeTest();
    byteCursorIsNotWithingRangeTest();

    getCurrentBitOfCharWithValidCipherTextAndValidCursorTest();
    getCurrentBitOfCharWithValidCipherTextAndValidCursorTest2();

    flipFirstLSBOfAZeroBitToZeroTest();
    flipFirstLSBOfAOneBitToZeroTest();

    lsb1WithAllOnesAndSomeBytesWithoutStegoTest();
    lsb1WithAllZerosAndSomeBytesWithoutStegoTest();
    lsb1WithZerosAndOnesWithAllBytesInStegoTest();
}