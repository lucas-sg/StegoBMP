#include "./include/testParser.h"
#include "./include/testRC4.h"
#include "./include/testLsbHelper.h"
#include "./include/testLsb.h"

int
main()
{
    printf("RUNNING PARSER TESTS\n");
    testParser();
    printf("RUNNING RC4 TESTS\n");
    testRC4();
    printf("RUNNING LSB HELPER TESTS \n");
    testLsbHelper();
    printf("RUNNING LSB TEST\n");
    testLsb();
}
