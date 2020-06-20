#ifndef LSBDECRYPT_H
#define LSBDECRYPT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

uint8_t *lsb1Decrypt(const uint8_t *bmpFile, const size_t cipherTextSize, const size_t bmpSize);

#endif