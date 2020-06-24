#ifndef LSBDECRYPT_H
#define LSBDECRYPT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t cipherTextSize, const size_t bmpSize, const size_t widthInPixels);
uint8_t *lsb4Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes);

#endif