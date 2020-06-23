#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdint.h>
#include "types.h"

int decrypt(const uint8_t *ciphertext, int ciphertextLen, const uint8_t *key, const uint8_t *iv, uint8_t *plaintext,
        ENCRYPTION encryption, ENC_MODE mode, const char *password);

#endif
