#ifndef CRYPTO_UTILS_Hextension
#define CRYPTO_UTILS_H

#include <openssl/evp.h>
#include "types.h"

const EVP_CIPHER *determineCipherAndMode(ENCRYPTION encryption, ENC_MODE mode);
size_t determineKeyLength(ENCRYPTION encryption);
void failedToCreateCipherContext();
void failedToInitCipherContext();
void failedToEncrypt();
void failedToDecrypt();
void failedToFinalizeEnc();
void failedToFinalizeDec();

#endif
