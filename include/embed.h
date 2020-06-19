#ifndef EMBED_H
#define EMBED_H

#include <stdint.h>
#include "types.h"

uint8_t *embed(uint8_t *carrierBmp, size_t carrierSize, const uint8_t *msg, size_t msgSize,
               UserInput userInput);

#endif
