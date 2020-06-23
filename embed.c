#include "embed.h"
#include "cryptoUtils.h"


uint8_t *
embed(uint8_t *carrierBmp, size_t carrierSize, const uint8_t *msg, size_t msgSize, UserInput userInput)
{
    uint8_t *outputBmp = NULL;

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
    int len, ciphertextLen;
//    const uint8_t *key = generateKey(password);
//    const uint8_t *iv  = generateIV(password);
// TODO: Delete the hardcoded key and IV and replace them for the real generator
    const uint8_t *key = (uint8_t *)"01234567890123456789012345678901";
    const uint8_t *iv  = (uint8_t *)"0123456789012345";
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        fprintf(stderr, "Failed to create cipher context\n");
        exit(0);
    }

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
    {
        fprintf(stderr, "Failed to initialize cipher context\n");
        exit(0);
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintextLen) != 1)
    {
        fprintf(stderr, "Failed to encrypt the provided plaintext\n");
        exit(0);
    }

    ciphertextLen = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
    {
        fprintf(stderr, "Failed to finalize the encryption process\n");
        exit(0);
    }

    ciphertextLen += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertextLen;
}