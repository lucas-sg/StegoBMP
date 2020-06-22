#include <stddef.h>
#include <openssl/evp.h>
#include "embed.h"


int encrypt(const uint8_t *plaintext, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode, const char *password);
const EVP_CIPHER *determineCipherAndMode(ENCRYPTION encryption, ENC_MODE mode);
const EVP_CIPHER *determineModeForAES128(ENC_MODE mode);
const EVP_CIPHER *determineModeForAES192(ENC_MODE mode);
const EVP_CIPHER *determineModeForAES256(ENC_MODE mode);
const EVP_CIPHER *determineModeForDES(ENC_MODE mode);
const uint8_t *generateKey(const char *password);
const uint8_t *generateIV(const char *password);


uint8_t *
embed(uint8_t *carrierBmp, size_t carrierSize, const uint8_t *msg, size_t msgSize, UserInput userInput)
{
    uint8_t *outputBmp = NULL;

    if (userInput.encryption != NONE)
        encrypt(msg, outputBmp, userInput.encryption, userInput.mode, userInput.password);

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
encrypt(const uint8_t *plaintext, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    EVP_CIPHER_CTX *ctx;
    int ciphertextLen, plaintextLen;
    const uint8_t *key = generateKey(password);
    const uint8_t *iv  = generateIV(password);
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        fprintf(stderr, "Failed to initialize cipher context\n");
        exit(0);    // TODO: Check if this is a valid way of exiting
    }

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv))
    {
        fprintf(stderr, "Failed to initialize the encryption operation\n");
        exit(0);    // TODO: Check if this is a valid way of exiting
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &ciphertextLen, plaintext, plaintextLen))
    {
        fprintf(stderr, "Failed to encrypt the provided plaintext\n");
        exit(0);    // TODO: Check if this is a valid way of exiting
    }

    ciphertextLen = plaintextLen;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + plaintextLen, &plaintextLen))
    {
        fprintf(stderr, "Failed to finalize the encryption process\n");
        exit(0);    // TODO: Check if this is a valid way of exiting
    }
    ciphertextLen += plaintextLen;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertextLen;
}

const EVP_CIPHER *
determineCipherAndMode(ENCRYPTION encryption, ENC_MODE mode)
{
    switch (encryption)
    {
        case AES_128:
            return determineModeForAES128(mode);
        case AES_192:
            return determineModeForAES192(mode);
        case AES_256:
            return determineModeForAES256(mode);
        case DES:
            return determineModeForDES(mode);
        case NONE:
        default:
            return NULL;
        // TODO: Ver si vale la pena poner esto solo para sacar el warning y si es asi, ver que hacer para no devolver algo tan villero
    }
}

const EVP_CIPHER *
determineModeForAES128(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_aes_128_ecb();
        case CFB:
            return EVP_aes_128_cfb1();  // TODO: Comparar con otros CFB
        case OFB:
            return EVP_aes_128_ofb();
        case CBC:
            return EVP_aes_128_cbc();   // TODO: Comparar con otros CBC
        default:
            break;
    }
}

const EVP_CIPHER *
determineModeForAES192(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_aes_192_ecb();
        case CFB:
            return EVP_aes_192_cfb1();  // TODO: Comparar con otros CFB
        case OFB:
            return EVP_aes_192_ofb();
        case CBC:
            return EVP_aes_192_cbc();
        default:
            break;
    }
}

const EVP_CIPHER *
determineModeForAES256(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_aes_256_ecb();
        case CFB:
            return EVP_aes_256_cfb1();  // TODO: Comparar con otros CFB
        case OFB:
            return EVP_aes_256_ofb();
        case CBC:
            return EVP_aes_256_cbc();
        default:
            break;
    }
}

const EVP_CIPHER *
determineModeForDES(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_des_ecb();
        case CFB:
            return EVP_des_cfb1();  // TODO: Comparar con otros CFB
        case OFB:
            return EVP_des_ofb();
        case CBC:
            return EVP_des_cbc();
        default:
            break;
    }
}

const uint8_t *
generateKey(const char *password)
{
    // TODO: Implementar como se obtiene la key en base a la password
}

const uint8_t *
generateIV(const char *password)
{
    // TODO: Implementar como se obtiene la key en base a la password
}