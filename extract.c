#include "include/extract.h"
#include "include/cryptoUtils.h"
#include <string.h>

OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsb4ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsbiExtractForPath(char *bmpPath, size_t bmpSize);

OUTPUT_BMP *
extract(char *bmpPath, size_t bmpSize, UserInput userInput)
{
    uint8_t *embeddedData, *plaintext, *originalMsg;
    size_t embeddedSize, originalMsgSize;

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            // TODO: Merge call to LSB1 implementation
//            embeddedSize = lsb1Extract(carrierBmp, carrierSize, embeddedData);
            break;
        case LSB4:
            // TODO: Merge call to LSB4 implementation
//            embeddedSize = lsb4Extract(carrierBmp, carrierSize, embeddedData);
            break;
        case LSBI:
            // TODO: Merge call to LSB1 implementation
//            embeddedSize = lsbiExtract(carrierBmp, carrierSize, embeddedData);
            break;
    }


    if (userInput.encryption != NONE)
    {
        plaintext = malloc(sizeof(*plaintext) * embeddedSize);
        decrypt(embeddedData, embeddedSize, plaintext, userInput.encryption, userInput.mode,
                userInput.password);
    }
    else
    {
        plaintext = embeddedData;
    }

    memcpy(&originalMsgSize, plaintext, 4);
    originalMsg = malloc(sizeof(*originalMsg) * originalMsgSize);
    memcpy(originalMsg, plaintext + 4, originalMsgSize);
    // TODO: Fix this file extension thing
    // strcpy(fileExtension, (char *)(plaintext + 4 + originalMsgSize));

    return originalMsg;
}


OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize)
{
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    uint8_t *decryption = lsb1Extract(bmpHeader->data, 102, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 102;
    return output;
}

OUTPUT_BMP *lsb4ExtractForPath(char *bmpPath, size_t bmpSize)
{
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    uint8_t *decryption = lsb4Extract(bmpHeader->data, 102, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 102;
    return output;
}

OUTPUT_BMP *lsbiExtractForPath(char *bmpPath, size_t bmpSize)
{
    BMP *bmpHeader = parseBmp(bmpPath);
    uint8_t *bmp = malloc(bmpHeader->header->size);
    uint8_t *decryption = lsbiExtract(bmpHeader->data, 102, bmpHeader->infoHeader->imageSize, bmpHeader->infoHeader->width * 3);
    OUTPUT_BMP *output = malloc(sizeof(OUTPUT_BMP));
    output->data = decryption;
    output->size = 102;
    return output;
}

int
decrypt(const uint8_t *ciphertext, int ctextLen, uint8_t *plaintext, ENCRYPTION encryption, ENC_MODE mode,
        const uint8_t *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, plaintextLen;
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);
    size_t keyLen = determineKeyLength(encryption);
    uint8_t *key  = malloc(keyLen);
    uint8_t *iv   = malloc(keyLen);
    EVP_BytesToKey(cipher, EVP_sha256(), NULL, password, (int)strlen((char *)password), 1, key, iv);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        failedToCreateCipherContext();

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        failedToInitCipherContext();

    if (EVP_DecryptUpdate(ctx, plaintext, &auxLen, ciphertext, ctextLen) != 1)
        failedToDecrypt();

    plaintextLen = auxLen;

    if (EVP_DecryptFinal_ex(ctx, plaintext + auxLen, &auxLen) != 1)
        failedToFinalizeDec();

    plaintextLen += auxLen;
    EVP_CIPHER_CTX_free(ctx);

    return plaintextLen;
}