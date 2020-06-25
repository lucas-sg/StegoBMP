#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>
#include "embed.h"
#include "types.h"

void extract(BMP *carrierBMP, MESSAGE *msg, UserInput userInput);
int decrypt(const uint8_t *ciphertext, int ciphertextLen, uint8_t *plaintext, ENCRYPTION encryption, ENC_MODE mode,
            const uint8_t *password);

#endif