#ifndef EMBED_H
#define EMBED_H

#include <stdint.h>
#include "../stego/lsbDecrypt.h"
#include "../stego/lsbEncrypt.h"
#include "./bmpParser.h"
#include "types.h"

uint8_t *embed(uint8_t *carrierBmp, size_t carrierSize, const char *msgPath, size_t msgSize,
               UserInput userInput);
uint8_t *lsb1Embed(const uint8_t *carrierBmp, const char *msgPath);

#endif
