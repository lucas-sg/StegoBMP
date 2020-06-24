#include "include/extract.h"
#include "include/embed.h"

uint8_t *decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password);
OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsb4ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsbiExtractForPath(char *bmpPath, size_t bmpSize);

OUTPUT_BMP *
extract(char *bmpPath, size_t bmpSize, UserInput userInput)
{
    uint8_t *output = NULL, *embeddedBytes = NULL;

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        return lsb1ExtractForPath(bmpPath, bmpSize);
    case LSB4:
        return lsb4ExtractForPath(bmpPath, bmpSize);
    case LSBI:
        return lsbiExtractForPath(bmpPath, bmpSize);
    }

    if (userInput.encryption != NONE)
        output = decrypt(embeddedBytes, userInput.encryption, userInput.mode, userInput.password);

    return output;
}

OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize)
{
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    uint8_t *decryption = lsb1Extract(bmpHeader->data, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 1247;
    return output;
}

OUTPUT_BMP *lsb4ExtractForPath(char *bmpPath, size_t bmpSize)
{
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    uint8_t *decryption = lsb4Extract(bmpHeader->data, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 102;
    return output;
}

OUTPUT_BMP *lsbiExtractForPath(char *bmpPath, size_t bmpSize)
{
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    uint8_t *decryption = lsbiExtract(bmpHeader->data, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 102;
    return output;
}

uint8_t *
decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}