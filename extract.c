#include "include/extract.h"
#include "include/cryptoUtils.h"
#include "include/lsbHelper.h"
#include <string.h>

OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsb4ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsbiExtractForPath(char *bmpPath, size_t bmpSize);
void copyFileExtension(MESSAGE *msg, uint8_t *sourceBytes);
void copyMsgData(MESSAGE *msg, uint8_t *sourceBytes);

// RC4KEY [TamañoEncripcion || encripcion(tamañoArchivo || datos || extension)]

EXTRACT_RET extract(BMP *carrierBMP, MESSAGE *msg, UserInput userInput)
{
    uint8_t *plaintext;
    uint8_t *pointerToBMPToExtractSize = userInput.stegoAlgorithm == LSBI ? carrierBMP->data + 6 : carrierBMP->data;
    size_t embeddedSize = extractFourBytesOfSizeFrom(pointerToBMPToExtractSize, userInput.stegoAlgorithm,
                                                     carrierBMP->infoHeader->imageSize);

    if (embeddedSize > carrierBMP->infoHeader->imageSize)
    {
        printf("The file you want to extract from this image is not embedded with "
               "the specified steganography algorithm\n");
        return WRONG_STEGO_ALGO_ERROR;
    }

    printf("El embedded size es %d\n", embeddedSize);
    uint8_t *embeddedMsg = calloc(embeddedSize, 1);

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            lsb1ExtractBytes(carrierBMP->data + 4 * SIZE_BYTES, embeddedMsg, embeddedSize);
            break;
        case LSB4:
            lsb4ExtractBytes(carrierBMP->data + 8, embeddedMsg, embeddedSize);
            break;
        case LSBI:
            lsbiExtractAndDecrypt(carrierBMP->data, embeddedMsg, carrierBMP->infoHeader->imageSize, embeddedSize);
            break;
    }
    printf("SALIO\n");
    if (userInput.encryption != NONE)
    {
        printf("Va a decriptar\n");
        plaintext = malloc(sizeof(*plaintext) * embeddedSize);
        decrypt(embeddedMsg, embeddedSize, plaintext, userInput.encryption, userInput.mode,
                userInput.password);
    }
    else
    {
        plaintext = embeddedMsg;
    }

    msg->data = plaintext;
    msg->size = embeddedSize;
    msg->extension = plaintext + embeddedSize;

    return EXTRACTION_SUCCEEDED;
}

int decrypt(const uint8_t *ciphertext, int ctextLen, uint8_t *plaintext, ENCRYPTION encryption, ENC_MODE mode,
            const uint8_t *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, plaintextLen;
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);
    size_t keyLen = determineKeyLength(encryption);
    uint8_t *key = malloc(keyLen);
    uint8_t *iv = malloc(keyLen);
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

void copyFileExtension(MESSAGE *msg, uint8_t *sourceBytes)
{
    char *fileExtension = (char *)(sourceBytes + 4 + msg->size);
    msg->extension = calloc(strlen(fileExtension) + 1, 1);
    strcpy((char *)msg->extension, fileExtension);
}

void copyMsgData(MESSAGE *msg, uint8_t *sourceBytes)
{
    msg->data = malloc(sizeof(*msg->data) * msg->size);
    memcpy(msg->data, sourceBytes + 4, msg->size);
}