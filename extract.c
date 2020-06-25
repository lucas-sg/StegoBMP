#include "include/extract.h"
#include "include/cryptoUtils.h"
#include "include/lsbHelper.h"
#include <string.h>

OUTPUT_BMP *lsb1ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsb4ExtractForPath(char *bmpPath, size_t bmpSize);
OUTPUT_BMP *lsbiExtractForPath(char *bmpPath, size_t bmpSize);

OUTPUT_BMP *
extract(BMP *carrierBMP, UserInput userInput)
{
    uint8_t *plaintext, *originalMsg;
    size_t embeddedSize, originalMsgSize;
    
    size_t stegoSize = extractStegoSizeFrom(carrierBMP->data);
    uint8_t *embeddedData = malloc(stegoSize);

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        // lsb1ExtractBytes(user);
        break;
    case LSB4:
        break;
    case LSBI:
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