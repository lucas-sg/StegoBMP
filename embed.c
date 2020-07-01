#include "include/embed.h"
#include "include/cryptoUtils.h"
#include "include/lsbEmbed.h"
#include "lsbHelper.h"
#include <string.h>

void lsbEmbed(STEGO_ALGO stegoAlgo, BMP *bmp, MESSAGE *msg);

void embed(UserInput userInput, BMP *carrierBmp, MESSAGE *msg)
{
    // TODO: Change this to allocate size for ptextLen + plaintext + fileExtension
    MESSAGE* msgToEmbed = malloc(sizeof(MESSAGE));
    msgToEmbed->extension = (uint8_t *) strdup((char*) msg->extension);
    uint8_t *inputSequence = malloc(getBytesNeededToStego(msg, userInput.stegoAlgorithm));
    size_t inputSeqLen = packMessage(msg->data, msg->size, (char *)msg->extension, inputSequence);

    if (userInput.encryption != NONE)
    {
        uint8_t *encPayload = malloc((msg->size / 16 + 1) * 16);
        size_t payloadSize = encrypt(inputSequence, inputSeqLen, encPayload, userInput.encryption, userInput.mode,
                            userInput.password);

        msgToEmbed->size = SIZE_BYTES + payloadSize;
        msgToEmbed->data = malloc(msgToEmbed->size);
        packEncPayload(encPayload, payloadSize, msgToEmbed->data);

        free(encPayload);
        free(inputSequence);
    }
    else
    {
        msgToEmbed->size = inputSeqLen;
        msgToEmbed->data = inputSequence;
    }

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        lsbEmbed(LSB1, carrierBmp, msgToEmbed);
        break;
    case LSB4:
        lsbEmbed(LSB4, carrierBmp, msgToEmbed);
        break;
    case LSBI:
        lsbEmbed(LSBI, carrierBmp, msgToEmbed);
        break;
    }

    destroyMsg(msgToEmbed);
}

void lsbEmbed(STEGO_ALGO stegoAlgo, BMP *bmp, MESSAGE *msg)
{
    if (getBytesNeededToStego(msg, stegoAlgo) > bmp->header->size)
        printf("The message you are trying to embed is too large for the .bmp carrier image (%d KB). "
               "Please choose a larger image or try to embed a smaller message.\n",
               (int)(bmp->header->size / 1024));

    switch (stegoAlgo)
    {
    // TODO: Change LSB1, LSB4 and LSBI prototype to lsbX(BMP *bmp, MESSAGE *msg);
    case LSB1:
        lsb1EmbedBytes(msg->data, bmp->data, msg->size);
        break;
    case LSB4:
        lsb4EmbedBytes(msg->data, bmp->data, msg->size);
        break;
    case LSBI:
        lsbiEncryptAndEmbed(msg->data, msg->size, bmp->data, bmp->infoHeader->imageSize);
        break;
    default:
        break;
    }
}

int encrypt(const uint8_t *plaintext, int ptextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode,
            const uint8_t *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, ciphertextLen;
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);
    size_t keyLen = determineKeyLength(encryption);
    uint8_t *key = malloc(keyLen);
    uint8_t *iv = malloc(keyLen);
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

    free(key);
    free(iv);

    return ciphertextLen;
}

size_t
prependSize(size_t size, uint8_t *buffer) {
    // First 4 bytes for size
    buffer[0] = (size & 0xFF000000) >> 24;
    buffer[1] = (size & 0x00FF0000) >> 16;
    buffer[2] = (size & 0x0000FF00) >> 8;
    buffer[3] = (size & 0x000000FF);

    return SIZE_BYTES;
}

size_t
packMessage(const uint8_t *data, size_t size,  const char *fileExtension, uint8_t *inputSequenceBuffer)
{
    size_t cursor = prependSize(size, inputSequenceBuffer);
    memcpy(inputSequenceBuffer + cursor, data, size);
    cursor += size;

    if(fileExtension != NULL && strlen(fileExtension) > 0)
    {
        sprintf((char *) inputSequenceBuffer + cursor, "%s", fileExtension);
        cursor += strlen(fileExtension) + 1;
    }

    // Total file size minus first 4 bytes used for file size :)
    return cursor;
}

size_t
packEncPayload(const uint8_t *data, size_t size, uint8_t *buffer)
{
    size_t cursor = prependSize(size, buffer);
    memcpy(buffer + cursor, data, size);
    cursor += size;
    return cursor;
}
