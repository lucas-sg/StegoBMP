//
// Created by andres on 6/19/20.
//

#include "../include/bmpParser.h"
#include "assert.h"
#include "stdio.h"

void testParseSampleImage() {
    char* path = "../tests/resources/sample.bmp";

    BMP_HEADER * header = parseBmp(path);

    assert(header->infoHeader->bits == 24);
    assert(header->infoHeader->width == 640);
    assert(header->infoHeader->height == 480);
}

void testgetBytesNeededToStegoForMessage() {
    char* path = "../tests/resources/message.txt";

    ulong result = getBytesNeededToStego(path, LSB1);

    // message.txt is 17 bytes. 4 bytes for size + 17 message + ".txt\0" (5) = 26 bytes to hide
    // LSB1 uses 1 byte per bit to hide. Answer should be 26 * 8 = 208
    assert(result == 208);
}

int main() {
    testParseSampleImage();
    testgetBytesNeededToStegoForMessage();
    printf("Tests passed!\n");
}
