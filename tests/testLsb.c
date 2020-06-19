#include <stdint.h>
#include "../stego/lsbEncrypt.h"

uint8_t *lsb1(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize);
uint8_t *lsb4(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize);
uint8_t replaceNthLSB(const uint8_t bmpByte, const uint8_t cipherTextByte, unsigned int cBitCursor, unsigned int bitToReplace);
uint8_t flippingNthLSBToZero(const uint8_t bytes, int bitToReplace);
uint8_t getCurrentBitOf(const uint8_t cipherTextuint8_t, unsigned int cBitCursor);
int isCursorWithinOneByteRange(unsigned int cursor);
void printingBits(const uint8_t byte);

static char *TEST_PASSED = "Test passed!!\n";
static char *RUNNING_TEST = "Running test: ";

void byteCursorIswithinRangeTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    unsigned int cursor = 8;

    assert(isCursorWithinOneByteRange(cursor) == 0);

    printf("%s ", TEST_PASSED);
    printf("Cursor %d is whithin the valid range of a byte cursor\n\n", cursor);
}

void byteCursorIsNotwithinRangeTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    unsigned int cursor = 23;

    assert(isCursorWithinOneByteRange(cursor) == 0);

    printf("%s ", TEST_PASSED);
    printf("Cursor %d is not whithin the valid range of a byte cursor\n\n", cursor);
}

void getCurrentFirstBitOfByteWithValidCiphertextAndValidCursorTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    unsigned int cBitCursor = 0;
    const uint8_t cipherTextByte = 0b0100001;

    assert(getCurrentBitOf(cipherTextByte, cBitCursor) == 0b00000001);

    printf("%s ", TEST_PASSED);
    printf("First LSB from cipherTextByte 0b0100001 is 1\n\n");
}

void getSecondBitOfByteWithValidCiphertextAndValidCursorTest()
{

    printf("%s %s \n", RUNNING_TEST, __func__);

    unsigned int cBitCursor = 1;
    const uint8_t cipherTextByte = 0b0100001;

    assert(getCurrentBitOf(cipherTextByte, cBitCursor) == 0b00000000);

    printf("%s ", TEST_PASSED);
    printf("First LSB from cipherTextByte 0b0100001 is 0\n\n");
}

void flipFirstLSBOfAZeroBitToZeroTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    unsigned int bitToReplace = 0;
    const uint8_t byte = 0b00001111;

    assert(flippingNthLSBToZero(byte, bitToReplace) == 0b00001110);

    printf("%s ", TEST_PASSED);
    printf("Last bit of 0b00001111 is now 0: 0b00001110\n\n");
}

void flipFirstLSBOfAOneBitToZeroTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    unsigned int bitToReplace = 2;
    const uint8_t byte = 0b00001111;

    assert(flippingNthLSBToZero(byte, bitToReplace) == 0b00001011);

    printf("%s ", TEST_PASSED);
    printf("Last bit of 0b00001111 is now 0: 0b00001011\n\n");
}

void lsb1WithAllOnesAndSomeBytesWithoutStegoTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t bmpFile[9] = {0b00011110, 0b11111110, 0b00011000,
                                0b00011000, 0b01010010, 0b11011110,
                                0b11011110, 0b01011110, 0b00011110};
    const uint8_t cipherText[1] = {0b11111111};

    const uint8_t *newBmpFile = lsb1(bmpFile, cipherText, 9, 1);
    for (int i = 8; i >= 0; i--)
    {
        if (i <= 8 && i >= 1)
        {
            assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(cipherText[0], 8 - i));
        }
        else
        {
            assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(bmpFile[i], 0));
        }
    }
    printf("%s ", TEST_PASSED);
    printf("LSB1 on current bmp file with c = 1^8 has worked as expected\n\n");
}

void lsb1WithZerosAndOnesWithAllBytesInStegoTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t bmpFile[9] = {0b00011111, 0b11111110, 0b00011001,
                                0b00011000, 0b01010011, 0b11011110,
                                0b11011111, 0b01011110};
    const uint8_t cipherText[1] = {0b01010101};

    const uint8_t *newBmpFile = lsb1(bmpFile, cipherText, 8, 1);
    for (int i = 7; i >= 0; i--)
    {
        assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(cipherText[0], 7 - i));
    }
    printf("%s ", TEST_PASSED);
    printf("LSB1 on current bmp file with c = (01)^4 has worked as expected\n\n");
}

void lsb1WithAllZerosAndSomeBytesWithoutStegoTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t bmpFile[9] = {0b00011111, 0b11111111, 0b00011001,
                                0b00011001, 0b01010011, 0b11011111,
                                0b11011111, 0b01011111, 0b00011110};
    const uint8_t cipherText[1] = {0b00000000};

    const uint8_t *newBmpFile = lsb1(bmpFile, cipherText, 9, 1);
    for (int i = 8; i >= 0; i--)
    {
        if (i <= 8 && i >= 1)
        {
            assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(cipherText[0], 8 - i));
        }
        else
        {
            assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(bmpFile[i], 0));
        }
    }
    printf("%s ", TEST_PASSED);
    printf("LSB1 on current bmp file with c=0^8 has worked as expected\n\n");
}

void lsbiFirstSixBytesUntouchedTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    const uint8_t bmpFile[19] = {
        0b00011111,
        0b11111111,
        0b00011001,
        0b00011001,
        0b01010011,
        0b11011111,
        0b11011111,
        0b01011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111
    };

    const uint8_t cipherText[1] = {0b00000000};

    const uint8_t *newBmpFile = lsbi(bmpFile, cipherText, 19, 1, 2);

    for (int i = 18 ; i > 12 ; i--) {
        assert(newBmpFile[i] == bmpFile[i]);
    }
    printf("%s ", TEST_PASSED);
    printf("Last 6 bytes of bmp file are intact (outside of lsbi range)\n\n");
}

void lsbiExtractionOkTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    const uint8_t bmpFile[19] = {
        0b00011111,
        0b11111111,
        0b00011001,
        0b00011001,
        0b01010011,
        0b11011111,
        0b11011111,
        0b01011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111,
        0b00011111
    };

    const uint8_t cipherText[1] = {0b00000000};

    const uint8_t *newBmpFile = lsbi(bmpFile, cipherText, 19, 1, 2);

    int indexesOfStego[8] = {12,10,8,6,4,2,0,11};

    for (int i = 0 ; i<= 7 ; i++) {
        assert(newBmpFile[indexesOfStego[i]] != bmpFile[indexesOfStego[i]]);
    }
    printf("%s ", TEST_PASSED);
    printf("Last 6 bytes of bmp file are intact (outside of lsbi range)\n\n");
}

int main()
{
    byteCursorIswithinRangeTest();
    byteCursorIsNotwithinRangeTest();

    getCurrentFirstBitOfByteWithValidCiphertextAndValidCursorTest();
    getSecondBitOfByteWithValidCiphertextAndValidCursorTest();

    flipFirstLSBOfAZeroBitToZeroTest();
    flipFirstLSBOfAOneBitToZeroTest();

    lsb1WithAllOnesAndSomeBytesWithoutStegoTest();
    lsb1WithAllZerosAndSomeBytesWithoutStegoTest();
    lsb1WithZerosAndOnesWithAllBytesInStegoTest();

    lsbiFirstSixBytesUntouchedTest();
    lsbiExtractionOkTest();
}