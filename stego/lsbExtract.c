#include "../include/lsbExtract.h"
#include "../include/lsbHelper.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void lsb1ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *fileExtension, size_t size);
void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size);
void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *fileExtension, size_t size);
void lsbiExtractEncryptedBytes(const uint8_t *bmp, size_t bmpSize, ENC_MESSAGE *encryptedMsg);


void lsb1Extract(const uint8_t *bmp, MESSAGE *msg)
{
    lsb1ExtractDataBytes(bmp, msg->data, msg->size);
    lsb1ExtractExtensionBytes(bmp, msg->extension, msg->size);
}

void lsb1ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    uint8_t extractedByte;

    for (size_t i = 0; i < size; i++)
    {
        extractedByte = lsb1ExtractByte(i, bmp);
        dst[i] = extractedByte;
    }
}

void lsb1ExtractExtensionBytes(const uint8_t *bmp, uint8_t *fileExtension, size_t size)
{
    uint8_t extractedByte = 1;

    for (size_t i = size; extractedByte != 0; i++)
    {
        extractedByte = lsb1ExtractByte(i, bmp);
        fileExtension[i - size] = extractedByte;
    }
}

void lsb4Extract(const uint8_t *bmp, MESSAGE *msg)
{
    lsb4ExtractDataBytes(bmp, msg->data, msg->size);
    lsb4ExtractExtensionBytes(bmp, msg->extension, msg->size);
}

void lsb4ExtractDataBytes(const uint8_t *bmp, uint8_t *dst, size_t size)
{
    for (size_t i = 0, j = 0; j < size; i += 2, j++)
    {
        uint8_t extractedByte = lsb4ExtractByte(i, bmp);
        dst[j] = extractedByte;
    }
}

void lsb4ExtractExtensionBytes(const uint8_t *bmp, uint8_t *fileExtension, size_t size)
{
    uint8_t extractedByte = 1;

    //  i = size * 2 because for each byte we want to extract, we need to go through 2 bmp bytes
    for (size_t i = size * 2, j = 0; extractedByte != 0; i += 2, j++)
    {
        extractedByte = lsb4ExtractByte(i, bmp);
        fileExtension[j] = extractedByte;
    }
}

void lsbiExtract(const uint8_t *bmp, size_t bmpSize, MESSAGE *msg)
{
    ENC_MESSAGE *encryptedMsg = malloc(sizeof(ENC_MESSAGE));
    encryptedMsg->size        = SIZE_BYTES + msg->size + MAX_EXTENSION_SIZE;
    encryptedMsg->data        = calloc(encryptedMsg->size, 1);

    lsbiExtractEncryptedBytes(bmp, bmpSize, encryptedMsg);

    uint8_t *decryptedMsg     = RC4(encryptedMsg->data, bmp, encryptedMsg->size);

    memcpy(msg->data, decryptedMsg + SIZE_BYTES, msg->size);
    copyFileExtension(msg->extension, decryptedMsg + SIZE_BYTES + msg->size);
}

void lsbiExtractEncryptedBytes(const uint8_t *bmp, size_t bmpSize, ENC_MESSAGE *encryptedMsg)
{
    size_t hop    = getHop(bmp[0]);
    size_t cursor = RC4_KEY_SIZE, laps = 0;

    for (uint32_t i = 0; i < encryptedMsg->size; i++)
    {
        uint8_t extractedByte   = lsbiExtractByte(bmp, bmpSize, &cursor, &laps, hop);
        (encryptedMsg->data)[i] = extractedByte;
    }
}

void lsb1ExtractEncryptedMsg(const uint8_t *bmp, ENC_MESSAGE *encMsg)
{
    lsb1ExtractDataBytes(bmp, encMsg->data, encMsg->size);
}

void lsb4ExtractEncryptedMsg(const uint8_t *bmp, ENC_MESSAGE *encMsg)
{
    lsb4ExtractDataBytes(bmp, encMsg->data, encMsg->size);
}

void lsbiExtractEncryptedMsg(const uint8_t *bmp, size_t bmpSize, ENC_MESSAGE *openSSLEncMsg)
{
    ENC_MESSAGE *rc4Msg = malloc(sizeof(ENC_MESSAGE));
    rc4Msg->size        = SIZE_BYTES + openSSLEncMsg->size;
    rc4Msg->data        = calloc(rc4Msg->size, 1);

    lsbiExtractEncryptedBytes(bmp, bmpSize, rc4Msg);

    uint8_t *decryptedRC4 = RC4(rc4Msg->data, bmp, rc4Msg->size);

    memcpy(openSSLEncMsg->data, decryptedRC4 + SIZE_BYTES, openSSLEncMsg->size);
}