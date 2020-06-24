#include "embed.h"
#include "cryptoUtils.h"
#include "lsbEmbed.h"
#include <string.h>


size_t buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer);


uint8_t *
embed(UserInput userInput, BMP *carrierBmp, MESSAGE *msg)
{
    uint8_t *outputBmp     = NULL;
    // TODO: Change this to allocate size for ptextLen + plaintext + fileExtension
    uint8_t *inputSequence = NULL;
    size_t inputSeqLen     = buildInputSequence(msg->data, msg->size, msg->extension, inputSequence);
    uint8_t *dataToEmbed;
    size_t dataLen;

    if (userInput.encryption != NONE)
    {
        dataToEmbed = malloc((msg->size/16 + 1) * 16);
        dataLen     = encrypt(inputSequence, inputSeqLen, dataToEmbed, userInput.encryption, userInput.mode,
                              userInput.password);
    }
    else
    {
        dataLen     = inputSeqLen;
        dataToEmbed = inputSequence;
    }

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            return lsbEmbed(LSB1, carrierBmp, dataToEmbed);
        case LSB4:
            return lsbEmbed(LSB4, carrierBmp, dataToEmbed);
        case LSBI:
            return lsbEmbed(LSBI, carrierBmp, dataToEmbed);
    }
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

int
encrypt(const uint8_t *plaintext, int ptextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode,
        const uint8_t *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, ciphertextLen;
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);
    size_t keyLen = determineKeyLength(encryption);
    uint8_t *key  = malloc(keyLen);
    uint8_t *iv   = malloc(keyLen);
    EVP_BytesToKey(cipher, EVP_sha256(), NULL, password, (int)strlen((char *)password), 1, key, iv);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        failedToCreateCipherContext();

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        failedToInitCipherContext();

    if (EVP_EncryptUpdate(ctx, ciphertext, &auxLen, plaintext, ptextLen) != 1)
        failedToEncrypt();

    ciphertextLen = auxLen;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + auxLen, &auxLen) != 1)
        failedToFinalizeEnc();

    ciphertextLen += auxLen;
    EVP_CIPHER_CTX_free(ctx);

    return ciphertextLen;
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
