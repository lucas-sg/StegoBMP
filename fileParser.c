#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/fileParser.h"

#define BITS_PER_PIXEL 24
void freeAll(int num, ...);

uint32_t
getBytesNeededToStego(MESSAGE * msg, STEGO_ALGO method)
{
    uint32_t sizeNeeded = 0;
    uint32_t packetSize = 4L + msg->size + strlen((char *) msg->extension) + 1;

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
        default:
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

MESSAGE* parseMessage(char* messagePath) {
    FILE* fd;

    if ((fd = fopen(messagePath, "r")) == NULL) {
        fprintf(stderr, "Unable to open MESSAGE file \"%s\"\n", messagePath);
        return NULL;
    }

    fseek(fd, 0L, SEEK_END);
    uint32_t messageSize = ftell(fd);
    rewind(fd);

    MESSAGE *msg = malloc(sizeof(MESSAGE));
    msg->size    = messageSize;
    msg->data    = malloc(messageSize);

    fread(msg->data, msg->size, 1, fd);
    fclose(fd);

    msg->extension = malloc(getExtensionSize(messagePath) + 1);
    strcpy((char *) msg->extension, strrchr(messagePath, '.'));

    return msg;
}

void saveBmp(BMP* bmp, char *bmpPath) {
    FILE *fp = fopen(bmpPath, "w+");

    fwrite(bmp->header, sizeof(HEADER), 1, fp);
    fwrite(bmp->infoHeader, sizeof(INFO_HEADER), 1, fp);
    fwrite(bmp->data, bmp->infoHeader->imageSize, 1, fp);

    fclose(fp);
}

void saveMessage(MESSAGE* msg, char *messagePathWithoutExtension) {
    printf("ASD\n");
    printf("en save messages %s %s \n", messagePathWithoutExtension, msg->extension);
    FILE *fp = fopen(strcat(messagePathWithoutExtension, msg->extension), "w+");

    fwrite(msg->data, msg->size, 1, fp);

    // fclose(fp);
}


uint32_t getExtensionSize(const char *fileName)
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