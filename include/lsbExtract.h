#ifndef LSBDECRYPT_H
#define LSBDECRYPT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "rc4.h"

void lsb1ExtractBytes(const uint8_t* src, uint8_t* dst, size_t size);
void lsb4ExtractBytes(const uint8_t* src, uint8_t* dst, size_t size);
size_t lsbiExtractBytes(const uint8_t* source, size_t sourceSize, uint8_t* dst, int hop);
void lsbiExtract(const uint8_t *source, uint8_t **dst, size_t sourceSize, size_t encryptedSize);

#endif