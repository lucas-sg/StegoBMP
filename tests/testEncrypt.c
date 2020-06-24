#include "include/testEncrypt.h"
#include "../include/embed.h"
#include "../include/extract.h"
#include <stdio.h>
#include <string.h>

void
testEncrypt()
{
    const uint8_t *plaintext = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean lorem urna, semper eu posuere luctus, ultrices sit amet lectus. Mauris aliquet pretium sem. Etiam quis tincidunt leo. Fusce velit nibh, bibendum eget bibendum ut, pulvinar ut eros. Duis in justo vitae felis accumsan varius. Sed hendrerit tempor gravida. Interdum et malesuada fames ac ante ipsum primis in faucibus.";
    const uint8_t *pass = "unused password";
    uint8_t ciphertext[1280];
    ENCRYPTION enc = AES_256;
    ENC_MODE mode = CBC;
    int cipherLen = encrypt(plaintext, (int)strlen((char *)plaintext), ciphertext, enc, mode, pass);
    printf("The ciphertext is:   %s\n", (char *)ciphertext);

    const uint8_t *key = (uint8_t *)"01234567890123456789012345678901";
    const uint8_t *iv  = (uint8_t *)"0123456789012345";
    uint8_t buffer[1280];
    char otherBuffer[1280];

    int plainLen = decrypt(ciphertext, cipherLen, buffer, enc, mode, pass);
    strncpy(otherBuffer, buffer, plainLen);
    printf("The plaintext is:   %s\n", otherBuffer);
}