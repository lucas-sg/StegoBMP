#include "include/extract.h"
#include "include/embed.h"

uint8_t *decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password);
OUTPUT_BMP *lsb1Extract(char *bmpPath, size_t bmpSize);

OUTPUT_BMP *
extract(char *bmpPath, size_t bmpSize, UserInput userInput)
{
    uint8_t *output = NULL, *embeddedBytes = NULL;

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        // TODO: Merge call to LSB1 implementation
        return lsb1Extract(bmpPath, bmpSize);
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

OUTPUT_BMP *lsb1Extract(char *bmpPath, size_t bmpSize)
{
    FILE *bmpFd = fopen(bmpPath, "r");
    uint8_t *bmp = malloc(bmpSize);
    fread(bmp, bmpSize, 1, bmpFd);

    uint8_t decryption = lsb1Decrypt(bmp, 19, bmpSize);

    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 19;

    return output;
}

uint8_t *
decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}