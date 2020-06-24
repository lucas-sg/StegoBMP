#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>
#include "embed.h"
#include "types.h"

OUTPUT_BMP *extract(char *bmpPath, size_t bmpSize, UserInput userInput);
int decrypt(const uint8_t *ciphertext, int ciphertextLen, uint8_t *plaintext, ENCRYPTION encryption, ENC_MODE mode,
            const uint8_t *password);

#endif