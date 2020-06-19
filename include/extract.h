#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"

uint8_t *extract(uint8_t *carrierBmp, size_t carrierSize, UserInput userInput);

#endif
