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
        //            outputBmp = lsb4Embed(carrierBmp, msg);
        break;
    case LSBI:
        // TODO: Merge call to LSB1 implementation
        //            outputBmp = lsbiEmbed(carrierBmp, msg);
        break;
    }

    return output;
}

OUTPUT_BMP *lsb1Embed(const uint8_t *carrierBmp, const char *bmpPath, const uint8_t *msg, const char *msgPath)
{
    printf("msgPath %s. BmpPath %s\n", msgPath, bmpPath);
    ulong bytesNeeded = getBytesNeededToStego(msgPath, LSB1);
    printf("Despues de bytes needed\n");
    BMP *bmpHeader = parseBmp(bmpPath);
    printf("bytes needed %ld\n", bytesNeeded);
    u_int32_t headerSize = bmpHeader->infoHeader->size;
    printf("Header size %d\n", headerSize);
    u_int32_t imgSize = bmpHeader->infoHeader->imageSize;
    printf("Img size %d\n", imgSize);
    u_int32_t offset = bmpHeader->header->offset;
    printf("Offset %d\n", headerSize);
    uint8_t *bmpFile = bmpHeader->data + offset;

    /**
     * this 19 should be unharcoded, the msg to stego should be of such format (see github issues)
     */
    printf("A punto de hacer lsb1 \n");
    uint8_t *bmpWithoutHeader = lsb1(bmpFile, msg, imgSize, 19);

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

    compareBothBmps(aux, fullBmp, bmpHeader->header->size);

    free(bmpWithoutHeader);
    return output;
}
// uint8_t *lsb1(const uint8_t *bmpFile, const uint8_t *cipherText, const size_t bmpFileSize, const size_t cipherTextSize)

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}

void compareBothBmps(uint8_t *orig, uint8_t *stegoBmp, size_t size)
{
    int iguales = 0;
    int distintos = 0;
    int primeros40Iguales = 0;
    for (int i = size - 1; i >= size - 145; i--)
    {
        if (i >= 0 && i <= 39)
        {
            if (orig[i] == stegoBmp[i])
                primeros40Iguales++;
        }
        else
        {
            if (orig[i] == stegoBmp[i])
            {
                iguales++;
            }
            else
            {
                distintos++;
            }
        }
    }
    printf("Iguales %d, disitntos %d, primeros40Iguales %d\n", iguales, distintos, primeros40Iguales);
    printf("Total %d, size: %d\n\n", iguales + distintos + primeros40Iguales, size);
}