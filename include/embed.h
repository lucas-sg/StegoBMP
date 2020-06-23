#ifndef EMBED_H
#define EMBED_H

#include "types.h"

uint8_t *embed(uint8_t *carrierBmp, size_t carrierSize, const uint8_t *msg, size_t msgSize, UserInput userInput);
int encrypt(const uint8_t *plaintext, int plaintextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode,
            const char *password);

#endif
