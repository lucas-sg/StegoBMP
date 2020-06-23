#include "embed.h"
#include "cryptoUtils.h"


uint8_t *
embed(uint8_t *carrierBmp, size_t carrierSize, const uint8_t *msg, size_t msgSize, UserInput userInput)
{
    uint8_t *outputBmp = NULL;
//    uint8_t *encryptedMsg = createEncBuffer();

    if (userInput.encryption != NONE)
        encrypt(msg, msgSize, outputBmp, userInput.encryption, userInput.mode, userInput.password);

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            // TODO: Merge call to LSB1 implementation
//            outputBmp = lsb1Embed(carrierBmp, msg);
            break;
        case LSB4:
            // TODO: Merge call to LSB4 implementation
//            outputBmp = lsb4Embed(carrierBmp, msg);
            break;
        case LSBI:
            // TODO: Merge call to LSB1 implementation
//            outputBmp = lsbiEmbed(carrierBmp, msg);
            break;
    }

    return outputBmp;
}

int
encrypt(const uint8_t *plaintext, int plaintextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, ciphertextLen;
//    const uint8_t *key = generateKey(password);
//    const uint8_t *iv  = generateIV(password);
// TODO: Delete the hardcoded key and IV and replace them for the real generator
    const uint8_t *key = (uint8_t *)"01234567890123456789012345678901";
    const uint8_t *iv  = (uint8_t *)"0123456789012345";
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        failedToCreateCipherContext();

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        failedToInitCipherContext();

    if (EVP_EncryptUpdate(ctx, ciphertext, &auxLen, plaintext, plaintextLen) != 1)
        failedToEncrypt();

    ciphertextLen = auxLen;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + auxLen, &auxLen) != 1)
        failedToFinalizeEnc();

    ciphertextLen += auxLen;
    EVP_CIPHER_CTX_free(ctx);

    return ciphertextLen;
}