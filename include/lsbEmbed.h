#ifndef LSBENCRYPT_H
#define LSBENCRYPT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "rc4.h"

uint8_t flippingNthLSBToZero(const uint8_t bytes, int bitToReplace);
uint8_t getCurrentBitOf(const uint8_t cipherTextuint8_t, unsigned int cBitCursor);
int isCursorWithinOneByteRange(unsigned int cursor);
void printingBits(int number);
void lsb1EmbedBytes(const uint8_t* src, uint8_t* dst, size_t size);
void lsb4EmbedBytes(const uint8_t* src, uint8_t* dst, size_t size);
void lsbiEncryptAndEmbed(const uint8_t* src, uint32_t msgSize, uint8_t* dst, size_t dstSize);
void lsbiEmbedBytes(const uint8_t* src, size_t msgSize, uint8_t* dst, size_t dstSize, size_t hop);
void lsbiEmbedSize(uint32_t msgSize, uint8_t* dst, size_t dstSize, size_t hop, int* cursor, int* laps);

#endif