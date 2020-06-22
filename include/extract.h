#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>
#include "embed.h"
#include "types.h"

OUTPUT_BMP *
extract(char *bmpPath, size_t bmpSize, UserInput userInput);

#endif
