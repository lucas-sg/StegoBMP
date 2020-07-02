#include "include/testLsbHelper.h"
#include "../include/lsbHelper.h"

void testExtractSizeFromLSB1();

void testLsbHelper()
{
        printf("Running LSB Helper test\n");
        testExtractSizeFromLSB1();
}

void testExtractSizeFromLSB1()
{
        // 0100 1101 0100 0010 0000 0000 0011 0110
        const uint8_t bmp[8 * SIZE_BYTES] =
            {
                0, 1, 0, 0, 1, 1, 0, 1,
                0, 1, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 1, 1, 0, 1, 1, 0};

        size_t size = extractSizeFromLSB1(bmp);

        assert(size == 1296171062);

        printf("\n[PASSED] Size was extracted correctly. The size is %ld \n", size);
}