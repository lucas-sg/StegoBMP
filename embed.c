#include <stddef.h>
#include "include/embed.h"

uint8_t    *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password);
OUTPUT_BMP *lsbEmbed(STEGO_ALGO stegoAlgo, BMP *carrierBmp, MESSAGE *msg);
OUTPUT_BMP *mergeBmpWithHeader(const uint8_t *bmpWithoutHeader, BMP *bmp);

OUTPUT_BMP *
embed(UserInput userInput, BMP *carrierBmp, MESSAGE *msg)
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
            return lsbEmbed(LSB1, carrierBmp, msgToEmbed);
        case LSB4:
            return lsbEmbed(LSB4, carrierBmp, msgToEmbed);
        case LSBI:
            return lsbEmbed(LSBI, carrierBmp, msgToEmbed);
    }

    return output;
}

OUTPUT_BMP *
lsbEmbed(STEGO_ALGO stegoAlgo, BMP *bmp, MESSAGE *msg)
{
    if (getBytesNeededToStego(msg, stegoAlgo) > bmp->header->size)
    {
        printf("The message you are trying to embed is too large for the .bmp carrier image (%d KB). "
               "Please choose a larger image or try to embed a smaller message.\n", (int) (bmp->header->size/1024));
        return NULL;
    }

    uint8_t *bmpWithoutHeader;

    switch (stegoAlgo)
    {
        // TODO: Change LSB1, LSB4 and LSBI prototype to lsbX(BMP *bmp, MESSAGE *msg);
        case LSB1:
//            bmpWithoutHeader = lsb1(bmp, msg);
        case LSB4:
//            bmpWithoutHeader = lsb4(bmp, msg);
        case LSBI:
//            bmpWithoutHeader = lsbi(bmp, msg);
        default:
            break;
    }

    return mergeBmpWithHeader(bmpWithoutHeader, bmp);
}

OUTPUT_BMP *
mergeBmpWithHeader(const uint8_t *bmpWithoutHeader, BMP *bmp)
{
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data       = malloc(bmp->header->size);
    memcpy(output->data, bmp->header, HEADER_SIZE);
    memcpy(output->data + HEADER_SIZE, bmpWithoutHeader, bmp->infoHeader->imageSize);
    output->size       = bmp->header->size;

    return output;
}

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}

size_t
buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer)
{
    // First 4 bytes for size
    ((uint32_t *) inputSequenceBuffer)[0] = size;
    size_t cursor = 4;

    memcpy(inputSequenceBuffer + cursor, data, size);
    cursor += size;

    sprintf((char *) inputSequenceBuffer + cursor, "%s", fileExtension);
    cursor += strlen(fileExtension) + 1;

    // Total file size minus first 4 bytes used for file size :)
    return cursor;
}
