//
// Created by andres on 6/18/20.
//

#ifndef STEGOBMP_BMPPARSER_H
#define STEGOBMP_BMPPARSER_H

#include <stdint.h>
#include "parser.h"

typedef struct __attribute__((packed))
{
    char type[2];   /* Magic identifier            */
    u_int32_t size; /* File size in bytes          */
    u_int16_t reserved1, reserved2;
    u_int32_t offset; /* Offset to image data, bytes */
} HEADER;

typedef struct __attribute__((packed))
{
    u_int32_t size;                     /* Header size in bytes      */
    int32_t width, height;              /* Width and height of image */
    u_int16_t planes;                   /* Number of colour planes   */
    u_int16_t bits;                     /* Bits per pixel            */
    u_int32_t compression;              /* Compression type          */
    u_int32_t imageSize;                /* Image size in bytes       */
    u_int32_t xResolution, yResolution; /* Pixels per meter          */
    u_int32_t nColors;                  /* Number of colours         */
    u_int32_t importantColors;          /* Important colours         */
} INFO_HEADER;

typedef struct
{
    HEADER *header;
    INFO_HEADER *infoHeader;
    uint8_t *data;
} BMP;

ulong getExtensionSize(const char *fileName);
ulong getBytesNeededToStego(const char *messagePath, STEGO_ALGO method);
BMP *parseBmp(char *bmpPath);

#endif //STEGOBMP_BMPPARSER_H
