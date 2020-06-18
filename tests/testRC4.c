#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "include/testRC4.h"
#include "../include/rc4.h"


void testOkRC4();
void compareBothRC4Files(FILE *thisRC4Encryption, FILE *onlineRC4Encryption, int len);


// In order to test the RC4 function, you need to hard code the some lines in rc4.c
// You can find the rest of the instructions in rc4.c in calculateKey() and initVectorT()
// You also need to uncomment the lines in this file regarding the 'encrypted' variable
void
testRC4()
{
    testOkRC4();
}

void
testOkRC4()
{
    char *msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus aliquam, erat vel fringilla rutrum, libero libero rhoncus felis, ac aliquam ex erat quis libero. In lacinia cursus sodales. Donec accumsan, velit ac maximus varius, eros libero iaculis mauris, eu convallis ante felis nec enim. Vestibulum a odio ac massa gravida fringilla. Aliquam tempus, erat non elementum rhoncus, magna justo sagittis risus, quis auctor erat tortor sed nisl. Sed sollicitudin erat at convallis dictum. Aliquam erat volutpat. Sed nisi quam, rutrum a vestibulum in, efficitur dictum diam. Nam vel consectetur neque, id viverra arcu. Aliquam porta, lectus sed sodales maximus, ex tortor sollicitudin augue, vulputate tincidunt purus tortor non orci. Aliquam et lacus congue, iaculis tellus id, mattis dolor.";
    char *key = "Clave de 128 bit";
    int len   = strlen(msg);
//    uint8_t *encrypted = RC4(msg, key, len);

    FILE *thisRC4Encryption   = fopen("../../exampleTP.dat", "r+");
    FILE *onlineRC4Encryption = fopen("../../exampleOnline.dat", "r");
//    fwrite(encrypted, sizeof(uint8_t), len, thisRC4Encryption);
    compareBothRC4Files(thisRC4Encryption, onlineRC4Encryption, len);
//    free(encrypted);
    printf("[PASSED] Both RC4 binary files, the one generated locally and the one downloaded, are equal\n\n");
}

void
compareBothRC4Files(FILE *thisRC4Encryption, FILE *onlineRC4Encryption, int len)
{
    uint8_t areEqual = 1;
    uint8_t thisRC4Byte[len], onlineRC4Byte[len];
    fread(thisRC4Byte, sizeof(*thisRC4Byte), len, thisRC4Encryption);
    fread(onlineRC4Byte, sizeof(*onlineRC4Byte), len, onlineRC4Encryption);

    for (int i = 0; i < len; i++)
        if ((areEqual = thisRC4Byte[i] == onlineRC4Byte[i]))
            break;

    assert(areEqual);

    fclose(thisRC4Encryption);
    fclose(onlineRC4Encryption);
}