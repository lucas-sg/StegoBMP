#ifndef BMP_PARSER_H
#define BMP_PARSER_H

#include <stdint.h>
#include "parser.h"
#define HEADER_SIZE 54

typedef struct __attribute__((packed))
{
    char type[2];   /* Magic identifier            */
    size_t size; /* File size in bytes          */
    uint16_t reserved1, reserved2;
    uint32_t offset; /* Offset to image data, bytes */
} HEADER;

typedef struct __attribute__((packed))
{
    size_t size;                       /* Header size in bytes      */
    int32_t width, height;             /* Width and height of image */
    uint16_t planes;                   /* Number of colour planes   */
    uint16_t bits;                     /* Bits per pixel            */
    uint32_t compression;              /* Compression type          */
    size_t imageSize;                  /* Image size in bytes       */
    uint32_t xResolution, yResolution; /* Pixels per meter          */
    uint32_t nColors;                  /* Number of colours         */
    uint32_t importantColors;          /* Important colours         */
} INFO_HEADER;

typedef struct
{
    HEADER      *header;
    INFO_HEADER *infoHeader;
    uint8_t     *data;
} BMP;

typedef struct
{
    size_t   size;
    uint8_t *extension;
    uint8_t *data;
} MESSAGE;

uint32_t getExtensionSize(const char *fileName);
uint32_t getBytesNeededToStego(MESSAGE * message, STEGO_ALGO method);
BMP *parseBmp(char *bmpPath);
MESSAGE *parseMessage(char *messagePath);
void saveBmp(BMP* bmp, char *bmpPath);
void saveMessage(MESSAGE* msg, char *messagePathWithoutExtension);

#endif
