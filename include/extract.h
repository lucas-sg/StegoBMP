#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>
#include "embed.h"
#include "types.h"

EXTRACT_RET extract(BMP *carrierBMP, MESSAGE *msg, UserInput userInput);
uint8_t *decrypt(const ENC_MESSAGE *encMsg, ENCRYPTION encryption, ENC_MODE mode, const uint8_t *password);

#endif