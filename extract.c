#include "extract.h"
#include "cryptoUtils.h"


uint8_t *
extract(uint8_t *carrierBmp, size_t carrierSize, UserInput userInput)
{
    uint8_t *output = NULL, *ciphertext = NULL;
    int ciphertextLen = 0;      // TODO: CHANGE THIS!!!
//    const uint8_t *key = generateKey(password);
//    const uint8_t *iv  = generateIV(password);
// TODO: Delete the hardcoded key and IV and replace them for the real generator
    const uint8_t *key = (uint8_t *)"01234567890123456789012345678901";
    const uint8_t *iv  = (uint8_t *)"0123456789012345";
    size_t bufferSize = 0;      // TODO: CHANGE THIS!!!
    uint8_t *plaintextBuffer = malloc(sizeof(*plaintextBuffer) * bufferSize);

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            // TODO: Merge call to LSB1 implementation
//            embeddedBytes = lsb1Extract(carrierBmp, msg);
            break;
        case LSB4:
            // TODO: Merge call to LSB4 implementation
//            embeddedBytes = lsb4Extract(carrierBmp, msg);
            break;
        case LSBI:
            // TODO: Merge call to LSB1 implementation
//            embeddedBytes = lsbiExtract(carrierBmp, msg);
            break;
    }

    if (userInput.encryption != NONE)
        output = decrypt(ciphertext, ciphertextLen, key, iv, plaintextBuffer, userInput.encryption, userInput.mode,
                         userInput.password);

    return output;
}

int
decrypt(const uint8_t *ciphertext, int ciphertextLen, const uint8_t *key, const uint8_t *iv, uint8_t *plaintext,
        ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    EVP_CIPHER_CTX *ctx;
    int len, plaintextLen;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        failedToCreateCipherContext();

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
        failedToInitCipherContext();

    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertextLen) != 1)
        failedToDecrypt();

    plaintextLen = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        failedToFinalizeDec();

    plaintextLen += len;
    EVP_CIPHER_CTX_free(ctx);

    return plaintextLen;
}