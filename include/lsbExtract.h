#ifndef LSBDECRYPT_H
#define LSBDECRYPT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "rc4.h"

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t cipherTextSize, const size_t bmpSize, const size_t widthInPixels);
uint8_t *lsb4Extract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes);
uint8_t *lsbiExtract(const uint8_t *bmpFile, const size_t stegoSize, const size_t bmpSize, const size_t widthInBytes);

void lsb1ExtractBytes(const uint8_t* src, uint8_t* dst, size_t size);
void lsb4ExtractBytes(const uint8_t* src, uint8_t* dst, size_t size);
size_t lsbiExtractBytes(const uint8_t* source, size_t sourceSize, uint8_t* dst, int hop);
void lsbiExtractAndDecrypt(const uint8_t* source, size_t sourceSize, uint8_t* dst);
#endif