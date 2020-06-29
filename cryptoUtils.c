#include "include/cryptoUtils.h"
#include <string.h>

const EVP_CIPHER *determineModeForAES128(ENC_MODE mode);
const EVP_CIPHER *determineModeForAES192(ENC_MODE mode);
const EVP_CIPHER *determineModeForAES256(ENC_MODE mode);
const EVP_CIPHER *determineModeForDES(ENC_MODE mode);
void failedTo(const char *errorMsg);

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
            return EVP_aes_128_cfb1();
        case OFB:
            return EVP_aes_128_ofb();
        case CBC:
            return EVP_aes_128_cbc();   // TODO: Comparar con otros CBC
        default:
            break;
    }

    return NULL;
}

const EVP_CIPHER *
determineModeForAES192(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_aes_192_ecb();
        case CFB:
            return EVP_aes_192_cfb1();
        case OFB:
            return EVP_aes_192_ofb();
        case CBC:
            return EVP_aes_192_cbc();
        default:
            break;
    }

    return NULL;
}

const EVP_CIPHER *
determineModeForAES256(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_aes_256_ecb();
        case CFB:
            return EVP_aes_256_cfb1();
        case OFB:
            return EVP_aes_256_ofb();
        case CBC:
            return EVP_aes_256_cbc();
        default:
            break;
    }

    return NULL;
}

const EVP_CIPHER *
determineModeForDES(ENC_MODE mode)
{
    switch (mode)
    {
        case ECB:
            return EVP_des_ecb();
        case CFB:
            return EVP_des_cfb1();
        case OFB:
            return EVP_des_ofb();
        case CBC:
            return EVP_des_cbc();
        default:
            break;
    }

    return NULL;
}

size_t
determineKeyLength(ENCRYPTION encryption)
{
    switch (encryption)
    {
        case AES_128:
            return 16;
        case AES_192:
            return 24;
        case AES_256:
            return 32;
        case DES:
            return 8;
        case NONE:
        default:
            return 0;
    }
}

void
failedToCreateCipherContext()
{
    failedTo("create cipher context");
}

void
failedToInitCipherContext()
{
    failedTo("initialize cipher context");
}

void
failedToEncrypt()
{
    failedTo("encrypt the provided plaintext");
}

void
failedToFinalizeEnc()
{
    failedTo("finalize the encryption process");
}

void
failedToDecrypt()
{
    failedTo("decrypt the provided ciphertext");
}

void
failedToFinalizeDec()
{
    failedTo("finalize the decryption process");
}

void
failedTo(const char *errorMsg)
{
    fprintf(stderr, "Failed to %s\n", errorMsg);
    exit(0);
}