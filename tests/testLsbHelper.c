#include "include/testLsbHelper.h"
#include "../include/lsbHelper.h"

void testExtractSizeFromLSB1();

void
testLsbHelper()
{
    testExtractSizeFromLSB1();
}

void
testExtractSizeFromLSB1()
{
    // 1101 0100 1000 1001 1110 0101 0010 1011
    // 0xD489E52B
    const uint8_t bmp[8 * SIZE_BYTES] =
            {
                    3, 3, 2, 3, 2, 3, 2, 2,     // 1, 1, 0, 1, 0, 1, 0, 0,
                    3, 2, 2, 2, 3, 2, 2, 3,     // 1, 0, 0, 0, 1, 0, 0, 1,
                    3, 3, 3, 2, 2, 3, 2, 3,     // 1, 1, 1, 0, 0, 1, 0, 1,
                    2, 2, 3, 2, 3, 2, 3, 3      // 0, 0, 1, 0, 1, 0, 1, 1
            };

    // 0100 1101 0100 0010 0000 0000 0011 0110
    const uint8_t bmp2[8 * SIZE_BYTES] =
            {
                    0, 1, 0, 0, 1, 1, 0, 1,
                    0, 1, 0, 0, 0, 0, 1, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 1, 1, 0, 1, 1, 0
            };

    size_t size = extractSizeFromLSB1(bmp2);
    // TODO: Add assertion comparing the int or uint values of expected size and extracted size
//    printf("%lx", size);
}