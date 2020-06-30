#include "include/extract.h"
#include "include/cryptoUtils.h"
#include "include/lsbHelper.h"
#include <string.h>

int isEmbeddedSizeLargerThanBmp(size_t embeddedSize, BMP *bmp, STEGO_ALGO stegoAlgo);
ENC_MESSAGE *extractEncryptedMsg(const uint8_t *bmp, uint32_t embeddedSize, UserInput userInput);
void buildMessage(MESSAGE *msg, const uint8_t *decryptedMsg);
uint8_t *decrypt(const ENC_MESSAGE *encMsg, ENCRYPTION encryption, ENC_MODE mode, const uint8_t *password);
void extractMsg(BMP *bmp, MESSAGE *msg, UserInput userInput);

EXTRACT_RET extract(BMP *carrierBMP, MESSAGE *msg, UserInput userInput)
{
    uint32_t embeddedSize = extractFourBytesOfSizeFrom(carrierBMP->data, userInput.stegoAlgorithm,
                                                       carrierBMP->infoHeader->imageSize);

    if (isEmbeddedSizeLargerThanBmp(embeddedSize, carrierBMP, userInput.stegoAlgorithm))
    {
        printf("The file you want to extract from this image is not embedded with "
               "the specified steganography algorithm\n");
        return WRONG_STEGO_ALGO_ERROR;
    }

    if (userInput.encryption != NONE)
    {
        ENC_MESSAGE *encryptedMsg = extractEncryptedMsg(carrierBMP->data, embeddedSize, userInput);
        uint8_t *decryptedMsg = decrypt(encryptedMsg, userInput.encryption, userInput.mode, userInput.password);

        buildMessage(msg, decryptedMsg);
        free(encryptedMsg);
        free(decryptedMsg);
    }
    else
    {
        msg->size = embeddedSize;
        msg->data = malloc(msg->size);
        msg->extension = calloc(MAX_EXTENSION_SIZE, 1);

        extractMsg(carrierBMP, msg, userInput);
    }

    return EXTRACTION_SUCCEEDED;
}

int isEmbeddedSizeLargerThanBmp(size_t embeddedSize, BMP *bmp, STEGO_ALGO stegoAlgo)
{
    int lsb1OrLsbi = (stegoAlgo == LSB1 || stegoAlgo == LSBI) && (embeddedSize * 8 > bmp->infoHeader->imageSize);
    int lsb4 = stegoAlgo == LSB4 && embeddedSize * 2 > bmp->infoHeader->imageSize;

    return lsb1OrLsbi || lsb4;
}

ENC_MESSAGE *extractEncryptedMsg(const uint8_t *bmp, uint32_t embeddedSize, UserInput userInput)
{
    ENC_MESSAGE *encryptedMsg = malloc(sizeof(ENC_MESSAGE));
    encryptedMsg->size = embeddedSize;
    encryptedMsg->data = malloc(encryptedMsg->size);

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        lsb1ExtractEncryptedMsg(bmp + 8 * SIZE_BYTES, encryptedMsg);
        break;
    case LSB4:
        lsb4ExtractEncryptedMsg(bmp + 8, encryptedMsg);
        break;
    case LSBI:
        // TODO: Implement this, and MAKE SURE that the embedded size was properly calculated
        //            lsbiExtractEncryptedMsg(bmp, bmpSize, encryptedMsg);
        break;
    }

    return encryptedMsg;
}

void extractMsg(BMP *bmp, MESSAGE *msg, UserInput userInput)
{
    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        lsb1Extract(bmp->data + 8 * SIZE_BYTES, msg);
        break;
    case LSB4:
        lsb4Extract(bmp->data + 8, msg);
        break;
    case LSBI:
        lsbiExtract(bmp->data, bmp->infoHeader->imageSize, msg);
        break;
    }
}

uint8_t *decrypt(const ENC_MESSAGE *encMsg, ENCRYPTION encryption, ENC_MODE mode, const uint8_t *password)
{
    EVP_CIPHER_CTX *ctx;
    //    int plaintextLen;
    int auxLen;
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);
    uint8_t *plaintext = calloc(encMsg->size, 1);
    size_t keyLen = determineKeyLength(encryption);
    uint8_t *key = malloc(keyLen);
    uint8_t *iv = malloc(keyLen);

    EVP_BytesToKey(cipher, EVP_sha256(), NULL, password, (int)strlen((char *)password), 1, key, iv);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        failedToCreateCipherContext();

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        failedToInitCipherContext();

    if (EVP_DecryptUpdate(ctx, plaintext, &auxLen, encMsg->data, encMsg->size) != 1)
        failedToDecrypt();

    //    plaintextLen = auxLen;

    if (EVP_DecryptFinal_ex(ctx, plaintext + auxLen, &auxLen) != 1)
        failedToFinalizeDec();

    //    plaintextLen += auxLen;
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

void buildMessage(MESSAGE *msg, const uint8_t *decryptedMsg)
{
    msg->size = getSizeFromPointer(decryptedMsg);
    msg->data = calloc(msg->size, 1);
    msg->extension = calloc(MAX_EXTENSION_SIZE, 1);

    memcpy(msg->data, decryptedMsg + SIZE_BYTES, msg->size);
    copyFileExtension(msg->extension, decryptedMsg + SIZE_BYTES + msg->size);
}