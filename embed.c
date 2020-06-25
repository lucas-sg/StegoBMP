#include "include/embed.h"
#include "include/cryptoUtils.h"
#include "include/lsbEmbed.h"
#include <string.h>


size_t buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer);
void lsbEmbed(STEGO_ALGO stegoAlgo, BMP *bmp, MESSAGE *msg);

void
embed(UserInput userInput, BMP *carrierBmp, MESSAGE *msg)
{
    uint8_t *outputBmp     = NULL;
    // TODO: Change this to allocate size for ptextLen + plaintext + fileExtension
    uint8_t *inputSequence = NULL;
    size_t inputSeqLen     = buildInputSequence(msg->data, msg->size, (char *)msg->extension, inputSequence);
    printf("BUILD SEQUECNE\n");
    uint8_t *dataToEmbed;
    size_t dataLen;

    if (userInput.encryption != NONE)
    {
        msg->data = malloc((msg->size/16 + 1) * 16);
        msg->size = encrypt(inputSequence, inputSeqLen, msg->data, userInput.encryption, userInput.mode,
                              userInput.password);
    }
    else
    {
        msg->size = inputSeqLen;
        msg->data = inputSequence;
    }

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            lsbEmbed(LSB1, carrierBmp, msg);
        case LSB4:
            lsbEmbed(LSB4, carrierBmp, msg);
        case LSBI:
            lsbEmbed(LSBI, carrierBmp, msg);
    }
}

void
lsbEmbed(STEGO_ALGO stegoAlgo, BMP *bmp, MESSAGE *msg)
{
    printf("EN LSB EMBED\n");
    if (getBytesNeededToStego(msg, stegoAlgo) > bmp->header->size)
        printf("The message you are trying to embed is too large for the .bmp carrier image (%d KB). "
               "Please choose a larger image or try to embed a smaller message.\n", (int) (bmp->header->size/1024));

    switch (stegoAlgo)
    {
        // TODO: Change LSB1, LSB4 and LSBI prototype to lsbX(BMP *bmp, MESSAGE *msg);
        case LSB1:
            lsb1EmbedBytes(msg->data, bmp->data, msg->size);
        case LSB4:
            lsb4EmbedBytes(msg->data, bmp->data, msg->size);
        case LSBI:
            lsbiEncryptAndEmbed(msg->data, msg->size, bmp->data, bmp->infoHeader->imageSize);
        default:
            break;
    }
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
    printf("NO LLEGA A RETORNAR BUILD INPUT SEQUENCE\n");
    return cursor;
}
