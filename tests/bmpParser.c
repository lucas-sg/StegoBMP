//
// Created by andres on 6/19/20.
//

#include "../include/bmpParser.h"
#include "assert.h"
#include "stdio.h"

void testParseSampleImage() {
    char* path = "../tests/resources/sample.bmp";

    BMP * bmp = parseBmp(path);

    assert(bmp->infoHeader->bits == 24);
    assert(bmp->infoHeader->width == 640);
    assert(bmp->infoHeader->height == 480);
}

void testBytesNeededForMessage() {
    char* path = "../tests/resources/message.txt";

    ulong result = getBytesNeededToStego(path, LSB1);

    // message.txt is 19 bytes. 4 bytes for size + 17 message + ".txt\0" (5) = 26 bytes to hide
    // LSB1 uses 1 byte per bit to hide. Answer should be 26 * 8 = 208
    assert(result == 208);
}

int main() {
    testParseSampleImage();
    testBytesNeededForMessage();
    printf("Tests passed!\n");
}
