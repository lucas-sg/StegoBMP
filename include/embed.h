#ifndef EMBED_H
#define EMBED_H

#include "types.h"

uint8_t *embed(uint8_t *carrierBmp, size_t carrierLen, const uint8_t *plaintext, size_t ptextLen,
               const char *fileExtension, UserInput userInput);
int encrypt(const uint8_t *plaintext, int plaintextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode,
            const char *password);

#endif
