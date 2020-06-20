//
// Created by andres on 6/18/20.
//
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/bmpParser.h"

#define BITS_PER_PIXEL 24
void freeAll(int num, ...);

ulong getBytesNeededToStego(const char *messagePath, STEGO_ALGO method)
{
    FILE *message;
    if ((message = fopen(messagePath, "r")) == NULL)
    {
        fprintf(stderr, "Unable to open message file \"%s\"\n", messagePath);
        return -1;
    }

    // Get size of message.
    fseek(message, 0L, SEEK_END);
    long messageSize = ftell(message);
    fclose(message);

    long sizeNeeded = 0;
    long packetSize = 4L + messageSize + getExtensionSize(messagePath) + 1;
    switch (method)
    {
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

BMP_HEADER *parseBmp(char *bmpPath, const uint8_t *bmpFile)
{
    FILE *bmp;
    if ((bmp = fopen(bmpPath, "r")) == NULL)
    {
        fprintf(stderr, "Unable to open BMP file \"%s\"\n", bmpPath);
        return NULL;
    }

    HEADER *header = malloc(sizeof(HEADER));
    INFO_HEADER *infoHeader = malloc(sizeof(INFO_HEADER));
    BMP_HEADER *bmpHeader = malloc(sizeof(BMP_HEADER));

    printf("LINEA 57\n");
    bmpHeader->header = header;
    printf("LINEA 59\n");
    bmpHeader->infoHeader = infoHeader;
    printf("LINEA 61\n");

    if (fread(header, sizeof(HEADER), 1, bmp) != 1)
    {
        printf("PINCHA 1\n");
        perror("Error reading HEADER.\n");
        freeAll(3, header, infoHeader, bmpHeader);
        return NULL;
    }

    if (fread(infoHeader, sizeof(INFO_HEADER), 1, bmp) != 1)
    {
        printf("PINCHA 2\n");
        perror("Error reading INFOHEADER.\n");
        freeAll(3, header, infoHeader, bmpHeader);
        return NULL;
    }

    if (infoHeader->bits != BITS_PER_PIXEL)
    {
        printf("PINCHA 3\n");
        fprintf(stderr, "Bits per pixel is %d. Expected %d.", infoHeader->bits, BITS_PER_PIXEL);
        freeAll(3, header, infoHeader, bmpHeader);
        return NULL;
    }

    bmpHeader->imgBytes = malloc(infoHeader->imageSize);
    memcpy(bmpHeader->imgBytes, bmpFile + (header->offset), infoHeader->imageSize);

    return bmpHeader;
}

ulong getExtensionSize(const char *fileName)
{
    return strlen(strrchr(fileName, '.'));
}

void freeAll(int num, ...)
{
    va_list vaList;

    va_start(vaList, num);
    for (int i = 0; i < num; i++)
    {
        free(va_arg(vaList, void *));
    }
    va_end(vaList);
}