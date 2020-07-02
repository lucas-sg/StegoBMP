#include "include/testEncrypt.h"
#include "../include/embed.h"
#include "../include/extract.h"
#include <stdio.h>
#include <string.h>

void
testEncryptDecrypt()
{
    // TODO: FIX THIS, THE TEST IS STALE DUE TO THE CHANGE IN THE DEFINITION OF decrypt() WHICH IS USED IN LINE 22
//    const char *plaintext = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean lorem urna, semper eu posuere luctus, ultrices sit amet lectus. Mauris aliquet pretium sem. Etiam quis tincidunt leo. Fusce velit nibh, bibendum eget bibendum ut, pulvinar ut eros. Duis in justo vitae felis accumsan varius. Sed hendrerit tempor gravida. Interdum et malesuada fames ac ante ipsum primis in faucibus.";
//    const char *pass = "unused password";
//    uint8_t ciphertext[1280];
//    ENCRYPTION enc = AES_256;
//    ENC_MODE mode  = CBC;
//    int cipherLen  = encrypt((uint8_t *)plaintext, (int)strlen(plaintext), ciphertext, enc, mode, (uint8_t *)pass);
//    printf("The ciphertext is:   %s\n", (char *)ciphertext);
//
//    char buffer[1280] = {0};
//    char otherBuffer[1280] = {0};
//
//    int plainLen = decrypt(ciphertext, cipherLen, (uint8_t *)buffer, enc, mode, (uint8_t *)pass);
//    strncpy(otherBuffer, buffer, plainLen);
//    printf("The plaintext is:   %s\n", otherBuffer);
}