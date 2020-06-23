#include "./include/testParser.h"
#include "./include/testRC4.h"
#include "./include/testEncrypt.h"

int
main(int argc, char *argv[])
{
    testEncrypt();
    testParser();
    testRC4();
}
