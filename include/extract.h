#ifndef EXTRACT_H
#define EXTRACT_H

#include "types.h"

uint8_t *extract(uint8_t *carrierBmp, size_t carrierSize, UserInput userInput);
int decrypt(const uint8_t *ciphertext, int ciphertextLen, const uint8_t *key, const uint8_t *iv, uint8_t *plaintext,
            ENCRYPTION encryption, ENC_MODE mode, const char *password);

#endif
