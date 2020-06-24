#ifndef LSBDECRYPT_H
#define LSBDECRYPT_H

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include "../include/lsbHelper.h"
#include "../include/lsbEmbed.h"

uint8_t *lsb1Extract(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInPixels);
uint8_t *lsb4Extract(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes);
uint8_t *lsbiExtract(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes);
uint8_t *extractRC4Key(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes);
int getHopFromBmpFile(const uint8_t *bmpFile, const size_t bmpSize, const size_t widthInBytes);
unsigned int extractDecimalFromBinary(uint8_t binary);

#endif