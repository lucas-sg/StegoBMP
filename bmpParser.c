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

BMP *parseBmp(char *bmpPath)
{
    FILE *bmpFd;

    if ((bmpFd = fopen(bmpPath, "r")) == NULL)
    {
        fprintf(stderr, "Unable to open BMP file \"%s\"\n", bmpPath);
        return NULL;
    }

    BMP *bmp = malloc(sizeof(BMP));
    bmp->header = malloc(sizeof(HEADER));
    bmp->infoHeader = malloc(sizeof(INFO_HEADER));

    if (fread(bmp->header, sizeof(HEADER), 1, bmpFd) != 1)
    {
        perror("Error reading HEADER.\n");
        freeAll(3, bmp->header, bmp->infoHeader, bmp);
        return NULL;
    }

    if (fread(bmp->infoHeader, sizeof(INFO_HEADER), 1, bmpFd) != 1)
    {
        perror("Error reading INFOHEADER.\n");
        freeAll(3, bmp->header, bmp->infoHeader, bmp);
        return NULL;
    }

    if (bmp->infoHeader->bits != BITS_PER_PIXEL)
    {
        fprintf(stderr, "Bits per pixel is %d. Expected %d.", bmp->infoHeader->bits, BITS_PER_PIXEL);
        freeAll(3, bmp->header, bmp->infoHeader, bmp);
        return NULL;
    }

    bmp->data = malloc(bmp->infoHeader->imageSize);
    if (fread(bmp->data, bmp->infoHeader->imageSize, 1, bmpFd) != 1) {
        perror("Error reading data.\n");
        freeAll(3, bmp->header, bmp->infoHeader, bmp->data, bmp);
        return NULL;
    }

    return bmp;
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