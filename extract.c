#include "include/extract.h"
#include "include/embed.h"

uint8_t *decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password);
OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize);

OUTPUT_BMP *
extract(char *bmpPath, size_t bmpSize, UserInput userInput)
{
    uint8_t *output = NULL, *embeddedBytes = NULL;

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        // TODO: Merge call to LSB1 implementation
        return lsb1ExtractForPath(bmpPath, bmpSize);
    case LSB4:
        // TODO: Merge call to LSB4 implementation
        //            embeddedBytes = lsb4Extract(carrierBmp, msg);
        break;
    case LSBI:
        // TODO: Merge call to LSB1 implementation
        //            embeddedBytes = lsbiExtract(carrierBmp, msg);
        break;
    }

    if (userInput.encryption != NONE)
        output = decrypt(embeddedBytes, userInput.encryption, userInput.mode, userInput.password);

    return output;
}

OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize)
{
    printf("LEVANTANDO EL ARCHIVITO\n");
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    printf("antes de extraer\n");
    uint8_t *decryption = lsb1Extract(bmpHeader->data, 102, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    printf("\n");
    // printf("Despues de extraer\n");
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 102;

    // for (int i = 0; i < 102; i++)
    // {
        // printf("%d | ", i);
        // printingBits(decryption[i]);
        // printf("%c ", decryption[i]);
    // }

    // printf("VA A RETORNAR\n");

    return output;
}

uint8_t *
decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}