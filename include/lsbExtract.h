#ifndef LSB_EXTRACT_H
#define LSB_EXTRACT_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "rc4.h"
// TODO: Change this, the message structure should be moved out of fileParser.h
#include "fileParser.h"

typedef struct
{
    uint32_t size;
    uint8_t *data;
} ENC_MESSAGE;

void lsb1Extract(const uint8_t* bmp, MESSAGE *msg);
void lsb4Extract(const uint8_t* bmp, MESSAGE *msg);
void lsbiExtract(const uint8_t *bmp, size_t bmpSize, MESSAGE *msg);
void lsb1ExtractEncryptedMsg(const uint8_t *bmp, ENC_MESSAGE *encMsg);
void lsb4ExtractEncryptedMsg(const uint8_t *bmp, ENC_MESSAGE *encMsg);
void lsbiExtractEncryptedMsg(const uint8_t *bmp, ENC_MESSAGE *encMsg);

#endif