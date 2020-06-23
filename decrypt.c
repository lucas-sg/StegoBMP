#include "decrypt.h"
#include <openssl/evp.h>

int
decrypt(const uint8_t *ciphertext, int ciphertextLen, const uint8_t *key, const uint8_t *iv, uint8_t *plaintext,
            ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    EVP_CIPHER_CTX *ctx;
    int len, plaintextLen;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        fprintf(stderr, "Failed to create cipher context\n");
        exit(0);
    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
    {
        fprintf(stderr, "Failed to initialize cipher context\n");
        exit(0);
    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertextLen) != 1)
    {
        fprintf(stderr, "Failed to decrypt the provided ciphertext\n");
        exit(0);
    }

    plaintextLen = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
    {
        fprintf(stderr, "Failed to finalize the encryption process\n");
        exit(0);
    }

    plaintextLen += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintextLen;
}