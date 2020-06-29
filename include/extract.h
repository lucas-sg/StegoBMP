#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>
#include "embed.h"
#include "types.h"

EXTRACT_RET extract(BMP *carrierBMP, MESSAGE *msg, UserInput userInput);

#endif