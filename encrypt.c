//
// Created by lucas on 22/6/20.
//

#include "encrypt.h"
#include <openssl/evp.h>


const EVP_CIPHER *determineCipherAndMode(ENCRYPTION encryption, ENC_MODE mode);
const EVP_CIPHER *determineModeForAES128(ENC_MODE mode);
const EVP_CIPHER *determineModeForAES192(ENC_MODE mode);
const EVP_CIPHER *determineModeForAES256(ENC_MODE mode);
const EVP_CIPHER *determineModeForDES(ENC_MODE mode);
const uint8_t *generateKey(const char *password);
const uint8_t *generateIV(const char *password);


int
encrypt(const uint8_t *plaintext, int plaintextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    EVP_CIPHER_CTX *ctx;
    int len, ciphertextLen;
//    const uint8_t *key = generateKey(password);
//    const uint8_t *iv  = generateIV(password);
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