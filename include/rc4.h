#ifndef RC4_H
#define RC4_H

#define KEY_SIZE 6
#include <stdlib.h>
#include <stdint.h>

uint8_t *RC4(const uint8_t *msg, const uint8_t *bmpFile, const size_t msgSize);
uint8_t *RC4decrypt(const uint8_t *encryptedMsg, const uint8_t *bmpCarrier, const size_t msgSize);

#endif
