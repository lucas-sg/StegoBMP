#include "./include/testParser.h"
#include "./include/testRC4.h"
#include "./include/testEncrypt.h"
#include "./include/testLsbHelper.h"
#include "./include/testLsb.h"

int
main()
{
    testEncryptDecrypt();
    testParser();
    testRC4();
    testLsbHelper();
    testLsb();
}
