#include <stddef.h>
#include "include/embed.h"

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password);

OUTPUT_BMP *
embed(uint8_t *carrierBmp, size_t carrierSize, const char *msgPath, size_t msgSize,
      UserInput userInput, const uint8_t *msg, const char *bmpPath)
{
    OUTPUT_BMP *output = NULL;

    uint8_t *msgToEmbed;

    if (userInput.encryption != NONE)
    {
        msgToEmbed = encrypt(msg, userInput.encryption, userInput.mode, userInput.password);
    }
    else
    {
        msgToEmbed = msg;
    }

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        // TODO: Merge call to LSB1 implementation
        return lsb1Embed(carrierBmp, bmpPath, msgToEmbed, msgPath);
    case LSB4:
        // TODO: Merge call to LSB4 implementation
        return lsb4Embed(carrierBmp, bmpPath, msgToEmbed, msgPath);
    case LSBI:
        // TODO: Merge call to LSB1 implementation
        //            outputBmp = lsbiEmbed(carrierBmp, msg);
        break;
    }

    return output;
}

// FIXME extract magic numbers to structs
OUTPUT_BMP *lsb1Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath)
{
    ulong bytesNeeded = getBytesNeededToStego(msgPath, LSB1);
    BMP *bmpHeader = parseBmp(bmpPath);
    u_int32_t headerSize = bmpHeader->infoHeader->size;
    u_int32_t imgSize = bmpHeader->infoHeader->imageSize;
    u_int32_t offset = bmpHeader->header->offset;
    uint8_t *bmpFile = bmpHeader->data;
    u_int32_t widthInBytes = bmpHeader->infoHeader->width * 3; // The *3 is because we asume pixels of 3 bytes

    /**
     * this size should be unharcoded, the msg to stego should be of such format (see github issues)
     */
    uint8_t *bmpWithoutHeader = lsb1(bmpFile, msg, imgSize, 102, widthInBytes);

    uint8_t *fullBmp = malloc(bmpHeader->header->size);

    // fix this
    uint8_t *aux = malloc(bmpHeader->header->size);
    FILE *bmpFd = fopen(bmpPath, "r");
    fread(aux, 1, bmpHeader->header->size, bmpFd);

    memcpy(fullBmp, aux, 54);
    memcpy(fullBmp + 54, bmpWithoutHeader, imgSize);

    OUTPUT_BMP *output = malloc(sizeof(OUTPUT));
    output->data = fullBmp;
    output->size = bmpHeader->header->size;

    free(bmpWithoutHeader);
    return output;
}


// FIXME extract magic numbers to structs, try to refactor so we have no duplicate code (function pointer)
OUTPUT_BMP *lsb4Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath)
{
    ulong bytesNeeded = getBytesNeededToStego(msgPath, LSB4);
    BMP *bmpHeader = parseBmp(bmpPath);
    u_int32_t headerSize = bmpHeader->infoHeader->size;
    u_int32_t imgSize = bmpHeader->infoHeader->imageSize;
    u_int32_t offset = bmpHeader->header->offset;
    uint8_t *bmpFile = bmpHeader->data;
    u_int32_t widthInBytes = bmpHeader->infoHeader->width * 3; // The *3 is because we asume pixels of 3 bytes

    /**
     * this size should be unharcoded, the msg to stego should be of such format (see github issues)
     */
    uint8_t *bmpWithoutHeader = lsb4(bmpFile, msg, imgSize, 102, widthInBytes);

    uint8_t *fullBmp = malloc(bmpHeader->header->size);

    // fix this
    uint8_t *aux = malloc(bmpHeader->header->size);
    FILE *bmpFd = fopen(bmpPath, "r");
    fread(aux, 1, bmpHeader->header->size, bmpFd);

    memcpy(fullBmp, aux, 54);
    memcpy(fullBmp + 54, bmpWithoutHeader, imgSize);

    OUTPUT_BMP *output = malloc(sizeof(OUTPUT));
    output->data = fullBmp;
    output->size = bmpHeader->header->size;

    free(bmpWithoutHeader);
    return output;
}

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}