#include <stdint.h>
#include <stdio.h>
#include "../include/embed.h"

void packMessageTest() {
    char* messagePath = "../tests/resources/message.txt";
    FILE* fp = fopen(messagePath, "r");

    uint8_t *message = malloc(100);
    fread(message, 18, 1, fp);

    size_t len = strlen((char*) message);

    uint8_t *buffer = malloc(100);
    size_t result = packMessage(message, len, ".txt", buffer);

    assert(result == 4 + len + 5);
    strcat((char*) message, ".txt");
    assert(strcmp((char*) buffer + 4, (char*) message) == 0);

    free(buffer);
}

void lsb1EmbedBytesTest() {
    uint8_t bytesToEmbed[2] = {1, 255};

    uint8_t *dst = calloc(0, 16);
    // Assign 8 1s at the beginning.
    dst[0] = 255;

    lsb1EmbedBytes(bytesToEmbed, dst, 2);

    // First byte was 255 so it should have only the last bit set to 0.
    assert(dst[0] == 254);
    // Remaining bytes were all 0.
    for(int i = 1; i < 7; i++ ){
        assert(dst[i] == 0);
    }
    assert(dst[7] == 1);
    for(int i = 8; i < 16; i++ ){
        assert(dst[i] == 1);
    }

    free(dst);
}

void lsb4EmbedBytesTest() {
    uint8_t bytesToEmbed[2] = {0x01, 0xFF};

    uint8_t *dst = calloc(0, 4);
    // Assign 8 1s at the beginning.
    dst[0] = 255;

    lsb4EmbedBytes(bytesToEmbed, dst, 2);

    // First byte was 255 so it should have only the first 4 bits set to 1.
    assert(dst[0] == 0b11110000);

    // Second byte was 0 so it should have the last bit set to 1.
    assert(dst[1] == 1);

    // Last 2 bytes were 0 so they should have the last 4 bits set to 1.
    assert(dst[2] == 0x0F);
    assert(dst[3] == 0x0F);

    free(dst);
}

void lsb1ExtractBytesTest() {
    uint8_t number = 17;
    char* extension = ".txt";
    uint8_t *buffer = malloc(10);
    size_t size = packMessage(&number, 1, extension, buffer);

    uint8_t *bmp = malloc(10 * 8);
    lsb1EmbedBytes(buffer, bmp, size);

    MESSAGE msg;
    msg.size = 1;
    msg.data = malloc(10);

    lsb1Extract(bmp + 8 * 4, &msg);

    assert(msg.data[0] == number);
    assert(strcmp(extension,(char*) msg.extension) == 0);
}

void lsb4ExtractBytesTest() {
    uint8_t number = 17;
    char* extension = ".txt";
    uint8_t *buffer = malloc(10);
    size_t size = packMessage(&number, 1, extension, buffer);

    uint8_t *bmp = malloc(10 * 8);
    lsb4EmbedBytes(buffer, bmp, size);

    MESSAGE msg;
    msg.size = 1;
    msg.data = malloc(10);

    lsb4Extract(bmp + 2 * 4, &msg);

    assert(msg.data[0] == number);
    assert(strcmp(extension,(char*) msg.extension) == 0);
}

void lsbiTest()
{
    uint8_t bmp[40] = {0};
    // Set hop to 2
    bmp[0] = 2;
    const uint8_t byte = 0xF0;
    lsbiEmbedBytes(&byte, 1, bmp, 40, bmp[0]);

    ENC_MESSAGE result;
    result.data = malloc(10);
    result.size = 1;

    lsbiExtractEncryptedBytes(bmp, 40, &result);

    assert(result.data[0] == byte);
}

void testLsb() {
    packMessageTest();

    lsb1EmbedBytesTest();
    lsb1ExtractBytesTest();

    lsb4EmbedBytesTest();
    lsb4ExtractBytesTest();

    lsbiTest();
}