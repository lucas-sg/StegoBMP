#ifndef RC4_H
#define RC4_H

#include <stdlib.h>
#include <stdint.h>

uint8_t *RC4(const uint8_t *msg, const uint8_t *bmpFile, const size_t msgSize);
uint8_t *RC4decrypt(const uint8_t *encryptedBmp, const size_t msgSize);

#endif
