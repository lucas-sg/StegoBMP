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
        const char *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, plaintextLen;
//    const uint8_t *key       = generateKey(password);
//    const uint8_t *iv        = generateIV(password);
    // TODO: Delete the hardcoded key and IV and replace them for the real generator
    const uint8_t *key = (uint8_t *)"01234567890123456789012345678901";
    const uint8_t *iv  = (uint8_t *)"0123456789012345";
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);

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