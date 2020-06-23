#include "include/testEncrypt.h"
#include "../include/encrypt.h"
#include "../include/decrypt.h"
#include <stdio.h>
#include <string.h>

void
testEncrypt()
{
    const uint8_t *plaintext = "The quick brown fox jumps over the lazy dog";
    const uint8_t *pass = "unused password";
    uint8_t ciphertext[128];
    ENCRYPTION enc = AES_256;
    ENC_MODE mode = CBC;
    int cipherLen = encrypt(plaintext, (int)strlen((char *)plaintext), ciphertext, enc, mode, pass);
    printf("The ciphertext is:   %s\n", (char *)ciphertext);

    const uint8_t *key = (uint8_t *)"01234567890123456789012345678901";
    const uint8_t *iv  = (uint8_t *)"0123456789012345";
    uint8_t buffer[128];
    char otherBuffer[64];

    int plainLen = decrypt(ciphertext, cipherLen, key, iv, buffer, enc, mode, pass);
    strncpy(otherBuffer, buffer, plainLen);
    printf("The plaintext is:   %s\n", otherBuffer);
}