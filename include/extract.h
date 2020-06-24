#ifndef EXTRACT_H
#define EXTRACT_H

#include "types.h"

uint8_t *extract(uint8_t *carrierBmp, size_t carrierSize, char *fileExtension, UserInput userInput);
int decrypt(const uint8_t *ciphertext, int ciphertextLen, uint8_t *plaintext, ENCRYPTION encryption, ENC_MODE mode,
            const uint8_t *password);

#endif