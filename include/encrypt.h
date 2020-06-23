#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "types.h"
#include <stdint.h>

int encrypt(const uint8_t *plaintext, int plaintextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode, const char *password);

#endif
