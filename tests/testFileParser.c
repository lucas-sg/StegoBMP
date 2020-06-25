#include "../include/fileParser.h"
#include "assert.h"
#include "string.h"
#include "stdio.h"

void testParseSampleImage() {
    char* path = "../tests/resources/sample.bmp";

    BMP * bmp = parseBmp(path);

    assert(bmp->infoHeader->bits == 24);
    assert(bmp->infoHeader->width == 640);
    assert(bmp->infoHeader->height == 480);
}

void testgetBytesNeededToStegoForMessage() {
    char* path = "../tests/resources/message.txt";

    MESSAGE * msg = parseMessage(path);
    uint32_t result = getBytesNeededToStego(msg, LSB1);

    // message.txt is 19 bytes. 4 bytes for size + 19 message + ".txt\0" (5) = 28 bytes to hide
    // LSB1 uses 1 byte per bit to hide. Answer should be 28 * 8 = 224
    assert(result == 224);
}

void testParseMessage() {
    char* path = "../tests/resources/message.txt";

    MESSAGE * msg = parseMessage(path);

    assert(strcmp((char *) msg->extension, ".txt") == 0);
    assert(msg->size == 19);
    assert(strcmp((char*) msg->data, "mensaje de prueba \n") == 0);
}

int main() {
    testParseSampleImage();
    testgetBytesNeededToStegoForMessage();
    testParseMessage();
    printf("Tests passed!\n");
}