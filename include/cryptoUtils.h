#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <openssl/evp.h>
#include "types.h"

const EVP_CIPHER *determineCipherAndMode(ENCRYPTION encryption, ENC_MODE mode);
const uint8_t *generateKey(const char *password);
const uint8_t *generateIV(const char *password);
void failedToCreateCipherContext();
void failedToInitCipherContext();
void failedToEncrypt();
void failedToDecrypt();
void failedToFinalizeEnc();
void failedToFinalizeDec();

#endif
