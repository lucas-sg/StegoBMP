#include "extract.h"
#include "cryptoUtils.h"
#include "stego/lsbDecrypt.h"
#include <string.h>


uint8_t *
extract(uint8_t *carrierBmp, size_t carrierSize, char *fileExtension, UserInput userInput)
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
    strcpy(fileExtension, (char *)(plaintext + 4 + originalMsgSize));

    return originalMsg;
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