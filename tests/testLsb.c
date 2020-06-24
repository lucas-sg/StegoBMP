#include <stdint.h>
#include <stdio.h>
#include "../include/embed.h"
#include "../include/lsbEmbed.h"
#include "../include/lsbExtract.h"
#include "../include/bmpParser.h"

static char *TEST_PASSED = "Test passed!!\n";
static char *RUNNING_TEST = "Running test: ";

void byteCursorIsWithinRangeTest()
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

    const uint8_t *newBmpFile = lsb1(bmpFile, cipherText, 9, 1, 3);

    const int bitsIndexThatShouldBeEqualToCipherBit[8] = {8, 7, 6, 5, 4, 3, 1, 0};
    const int bitsThatShouldBeEqualToOrigBmp[1] = {2};

    for (int i = 0; i <= 7; i++)
    {
        assert(getCurrentBitOf(cipherText[0], i) == getCurrentBitOf(newBmpFile[bitsIndexThatShouldBeEqualToCipherBit[i]], 0));
    }

    for (int i = 0; i <= 0; i++)
    {
        assert(getCurrentBitOf(bmpFile[bitsThatShouldBeEqualToOrigBmp[i]], 0) == getCurrentBitOf(newBmpFile[bitsThatShouldBeEqualToOrigBmp[i]], 0));
    }

    printf("%s ", TEST_PASSED);
    printf("LSB1 on current bmp file with c = 1^8 has worked as expected\n\n");
}

void lsb1WithZerosAndOnesWithAllBytesInStegoTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t bmpFile[9] = {0b00011111, 0b00011111, 0b11111110,
                                0b00011001, 0b00011000, 0b01010011,
                                0b11011110, 0b11011111, 0b01011110};
    const uint8_t cipherText[1] = {0b01010101};

    const uint8_t *newBmpFile = lsb1(bmpFile, cipherText, 8, 1, 3);

    const int bitsIndexThatShouldBeEqualToCipherBit[8] = {8, 7, 6, 5, 4, 3, 1, 0};
    const int bitsThatShouldBeEqualToOrigBmp[1] = {2};

    for (int i = 8; i <= 0; i++)
    {
        assert(getCurrentBitOf(cipherText[0], i) == getCurrentBitOf(newBmpFile[bitsIndexThatShouldBeEqualToCipherBit[i]], 0));
    }

    for (int i = 0; i <= 0; i++)
    {
        assert(getCurrentBitOf(bmpFile[bitsThatShouldBeEqualToOrigBmp[i]], 0) == getCurrentBitOf(newBmpFile[bitsThatShouldBeEqualToOrigBmp[i]], 0));
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

    const uint8_t *newBmpFile = lsb1(bmpFile, cipherText, 9, 1, 1);
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

void lsb1EmbedFullTest()
{
    BMP *bmpHeader = parseBmp("/home/tomas/workspace/StegoBMP/tests/resources/sample.bmp");
    FILE *msgFile = fopen("/home/tomas/workspace/StegoBMP/tests/resources/newMessage.txt", "r");
    uint8_t *msg = malloc(102);
    read(msg, sizeof(*msg), 102, msgFile);

    for (int i = 0; i < 102; i++)
    {
        printingBits(msg[i]);
    }

    const uint8_t *newBmpFile = lsb1(bmpHeader->data, msg, bmpHeader->infoHeader->imageSize, 102, bmpHeader->infoHeader->width * 3);
    int a = 0;
    for (int i = bmpHeader->infoHeader->imageSize - 1, j = 0; i >= 0; i--)
    {
        printf("EN EL FOR i= %d. j=%d\n", i, j);
        printingBits(newBmpFile[i]);
        printingBits(msg[j]);
        if (a == 8)
        {
            a = 0;
            j++;
        }
        if (i <= bmpHeader->infoHeader->imageSize - 1 && i >= bmpHeader->infoHeader->imageSize - 988)
        {
            assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(msg[j], a));
            a++;
        }
        else
        {
            assert(getCurrentBitOf(newBmpFile[i], 0) == getCurrentBitOf(bmpHeader->data[i], 0));
        }
    }
}

void lsb1ExtractTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t bmpFile[9] = {0b11011111, 0b11111111, 0b00011000,
                                0b00011001, 0b01010011, 0b11011111,
                                0b11011110, 0b01011111, 0b00011110};

    const uint8_t cipherText[1] = {0b01011111};

    const uint8_t *extractedCiphertext = lsb1Extract(bmpFile, 1, 3);

    assert(extractedCiphertext[0] == cipherText[0]);

    printf("%s", TEST_PASSED);
    printf("LSB1 extraction on current bmp file has worked as expected and it is equal to desire cipherText\n\n");
}

void inputSequenceTest() {
    char* messagePath = "../tests/resources/message.txt";
    FILE* fp = fopen(messagePath, "r");

    uint8_t *message = malloc(100);
    fread(message, 18, 1, fp);

    size_t len = strlen((char*) message);

    uint8_t *buffer = malloc(100);
    size_t result = buildInputSequence(message, len, ".txt", buffer);

    assert(result == 4 + len + 5);
    strcat((char*) message, ".txt");
    assert(strcmp((char*) buffer + 4, (char*) message) == 0);
}


void extractStegoSizeFromBigEndianTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t stegoSize[4] = {0x00, 0x00, 0x42, 0xDE};
    const int extractedStegoSize = extractStegoSizeFrom(stegoSize);
    assert(extractedStegoSize == 17118);
    printf("%s", TEST_PASSED);
    printf("Extracted stego size from [0x00, 0x00, 0x42, 0xDE] is 17118\n\n");
}

void extractStegoSizeFromLittleEndianTest()
{

    printf("%s %s \n", RUNNING_TEST, __func__);
    const uint8_t stegoSize[4] = {
        0xDE,
        0x42,
        0x00,
        0x00,
    };
    const int extractedStegoSize = extractStegoSizeFrom(stegoSize);
    assert(extractedStegoSize != 17118);
    printf("%s", TEST_PASSED);
    printf("Extracted stego size from [0xDE, 0x42, 0x00, 0x00] is NOT 17118\n\n");
}

void lsb1ExtractTest()
{
    printf("%s %s \n", RUNNING_TEST, __func__);

    const int withInBytes = 6;

    printf("%s", TEST_PASSED);
}

int main()
{
    byteCursorIsWithinRangeTest();
    byteCursorIsNotwithinRangeTest();

    getCurrentFirstBitOfByteWithValidCiphertextAndValidCursorTest();
    getSecondBitOfByteWithValidCiphertextAndValidCursorTest();

    flipFirstLSBOfAZeroBitToZeroTest();
    flipFirstLSBOfAOneBitToZeroTest();

    extractStegoSizeFromBigEndianTest();
    extractStegoSizeFromLittleEndianTest();

    lsb1ExtractTest();
    lsb4ExtractTest();
    lsbiExtractTest();

    // FIX ALL OF THESE TESTS
    // lsb1WithAllOnesAndSomeBytesWithoutStegoTest();
    // lsb1WithAllZerosAndSomeBytesWithoutStegoTest();
    // lsb1WithZerosAndOnesWithAllBytesInStegoTest();

    // lsb1ExtractTest();

    inputSequenceTest();
    // lsb1EmbedFullTest();
}