#ifndef RC4_H
#define RC4_H

#include <stdlib.h>
#include <stdint.h>

#define KEY_SIZE 6

uint8_t *RC4(const uint8_t *msg, const uint8_t *bmpFile, size_t msgSize);

#endif
