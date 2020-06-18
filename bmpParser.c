//
// Created by andres on 6/18/20.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "bmpParser.h"

#define BITS_PER_PIXEL 24

ulong bytesNeededForMessage(char* messagePath, STEGO_ALGO method) {
    FILE *message;
    if ((message = fopen(messagePath,"r")) == NULL) {
        fprintf(stderr,"Unable to open message file \"%s\"\n", messagePath);
        return -1;
    }

    // Get size of message.
    fseek(message, 0L, SEEK_END);
    long messageSize = ftell(message);
    fclose(message);

    long sizeNeeded = 0;
    long packetSize = 4L + messageSize + getExtensionSize(messagePath) + 1;
    switch (method) {
        case LSB1:
            sizeNeeded = packetSize * 8;
            break;
        case LSB4:
            sizeNeeded = packetSize * 2;
            break;
        case LSBI:
            sizeNeeded = packetSize * 8;
            break;
    }

    return sizeNeeded;
}

uint bmpSize(char* bmpPath) {
    HEADER header;
    INFO_HEADER infoHeader;

    FILE *bmp;

    if ((bmp = fopen(bmpPath,"r")) == NULL) {
        fprintf(stderr,"Unable to open BMP file \"%s\"\n", bmpPath);
        return -1;
    }

    if(fread(&header, sizeof(HEADER), 1, bmp) != 1) {
        perror("Error reading HEADER.\n");
        return -1;
    }

    if(fread(&infoHeader, sizeof(INFO_HEADER), 1, bmp) != 1) {
        perror("Error reading INFOHEADER.\n");
        return -1;
    }

    if(infoHeader.bits != BITS_PER_PIXEL) {
        fprintf(stderr, "Bits per pixel is %d. Expected %d.", infoHeader.bits, BITS_PER_PIXEL);
        return -1;
    }

    return infoHeader.imageSize;
}

ulong getExtensionSize(char *fileName) {
    return strlen(strrchr(fileName, '.'));
}