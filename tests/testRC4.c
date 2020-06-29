#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "include/testRC4.h"
#include "../include/rc4.h"


void testOkRC4();
void testDecryptRC4();
void compareBothRC4Files(const uint8_t *theseRC4Bytes, const uint8_t *onlineRC4Bytes, size_t len);


void
testRC4()
{
    testOkRC4();
    testDecryptRC4();
}

void
testOkRC4()
{
    const char *msg           = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus aliquam, erat vel fringilla rutrum, libero libero rhoncus felis, ac aliquam ex erat quis libero. In lacinia cursus sodales. Donec accumsan, velit ac maximus varius, eros libero iaculis mauris, eu convallis ante felis nec enim. Vestibulum a odio ac massa gravida fringilla. Aliquam tempus, erat non elementum rhoncus, magna justo sagittis risus, quis auctor erat tortor sed nisl. Sed sollicitudin erat at convallis dictum. Aliquam erat volutpat. Sed nisi quam, rutrum a vestibulum in, efficitur dictum diam. Nam vel consectetur neque, id viverra arcu. Aliquam porta, lectus sed sodales maximus, ex tortor sollicitudin augue, vulputate tincidunt purus tortor non orci. Aliquam et lacus congue, iaculis tellus id, mattis dolor.";
    const char *key           = "CRIPTO";
    size_t len                = strlen(msg);
    uint8_t *encrypted        = RC4((uint8_t *)msg, (uint8_t *)key, len);
    FILE *onlineRC4Encryption = fopen("../onlineRC4example.dat", "r");
    uint8_t onlineRC4Bytes[len];

    fread(onlineRC4Bytes, sizeof(*onlineRC4Bytes), len, onlineRC4Encryption);
    compareBothRC4Files(encrypted, onlineRC4Bytes, len);
    fclose(onlineRC4Encryption);
    free(encrypted);
    printf("[PASSED] Both RC4 binary files, the one generated locally and the one downloaded, are equal\n\n");
}

void testDecryptRC4()
{
    const char *msg           = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus aliquam, erat vel fringilla rutrum, libero libero rhoncus felis, ac aliquam ex erat quis libero. In lacinia cursus sodales. Donec accumsan, velit ac maximus varius, eros libero iaculis mauris, eu convallis ante felis nec enim. Vestibulum a odio ac massa gravida fringilla. Aliquam tempus, erat non elementum rhoncus, magna justo sagittis risus, quis auctor erat tortor sed nisl. Sed sollicitudin erat at convallis dictum. Aliquam erat volutpat. Sed nisi quam, rutrum a vestibulum in, efficitur dictum diam. Nam vel consectetur neque, id viverra arcu. Aliquam porta, lectus sed sodales maximus, ex tortor sollicitudin augue, vulputate tincidunt purus tortor non orci. Aliquam et lacus congue, iaculis tellus id, mattis dolor.";
    const char *key           = "CRIPTO";
    size_t len                = strlen(msg);
    FILE *onlineRC4Encryption = fopen("../onlineRC4example.dat", "r");
    uint8_t onlineRC4Bytes[len];

    fread(onlineRC4Bytes, sizeof(*onlineRC4Bytes), len, onlineRC4Encryption);
    uint8_t *decrypted = RC4(onlineRC4Bytes, (uint8_t *) key, 4);
    fclose(onlineRC4Encryption);
    printf("[PASSED] RC4 decryption\n\n");
}

void
compareBothRC4Files(const uint8_t *theseRC4Bytes, const uint8_t *onlineRC4Bytes, size_t len)
{
    uint8_t areEqual = 1;

    for (int i = 0; i < len; i++)
        if (!(areEqual = theseRC4Bytes[i] == onlineRC4Bytes[i]))
            break;

    assert(areEqual);
}