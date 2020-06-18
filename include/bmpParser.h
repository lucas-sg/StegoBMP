//
// Created by andres on 6/18/20.
//

#ifndef STEGOBMP_BMPPARSER_H
#define STEGOBMP_BMPPARSER_H

#include "parser.h"

typedef struct {
    unsigned short int type;                 /* Magic identifier            */
    unsigned int size;                       /* File size in bytes          */
    unsigned short int reserved1, reserved2;
    unsigned int offset;                     /* Offset to image data, bytes */
} HEADER;

typedef struct {
    unsigned int size;               /* Header size in bytes      */
    int width, height;                /* Width and height of image */
    unsigned short int planes;       /* Number of colour planes   */
    unsigned short int bits;         /* Bits per pixel            */
    unsigned int compression;        /* Compression type          */
    unsigned int imageSize;          /* Image size in bytes       */
    int xResolution, yResolution;     /* Pixels per meter          */
    unsigned int nColors;           /* Number of colours         */
    unsigned int importantColors;   /* Important colours         */
} INFO_HEADER;

ulong getExtensionSize(char *fileName);
ulong bytesNeeded(char* messagePath, STEGO_ALGO method);
uint bmpSize(char* bmpPath);

#endif //STEGOBMP_BMPPARSER_H
