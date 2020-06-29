#ifndef LSB_EXTRACT_H
#define LSB_EXTRACT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "rc4.h"

void lsb1Extract(const uint8_t* src, uint8_t* dst, size_t size);
void lsb4Extract(const uint8_t* src, uint8_t* dst, size_t size);
void lsbiExtract(const uint8_t *source, uint8_t **dst, size_t sourceSize, size_t encryptedSize);

#endif