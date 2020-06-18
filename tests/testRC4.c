#include <stdio.h>
#include <string.h>
#include "include/testRC4.h"
#include "../include/rc4.h"


void testOkRC4();

// In order to test the RC4 function, you need to hardcode the some lines in rc4.c
// You can find the rest of the instructions in rc4.c in calculateKey() and initVectorT()
void
testRC4()
{
//    testOkRC4();
}

void
testOkRC4()
{
    char *msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus aliquam, erat vel fringilla rutrum, libero libero rhoncus felis, ac aliquam ex erat quis libero. In lacinia cursus sodales. Donec accumsan, velit ac maximus varius, eros libero iaculis mauris, eu convallis ante felis nec enim. Vestibulum a odio ac massa gravida fringilla. Aliquam tempus, erat non elementum rhoncus, magna justo sagittis risus, quis auctor erat tortor sed nisl. Sed sollicitudin erat at convallis dictum. Aliquam erat volutpat. Sed nisi quam, rutrum a vestibulum in, efficitur dictum diam. Nam vel consectetur neque, id viverra arcu. Aliquam porta, lectus sed sodales maximus, ex tortor sollicitudin augue, vulputate tincidunt purus tortor non orci. Aliquam et lacus congue, iaculis tellus id, mattis dolor.";
    char *key = "Clave de 128 bit";
    uint8_t *encrypted = RC4(msg, key, strlen(msg));

    FILE *fptr = fopen("./exampleTP.dat", "wb+");
    fwrite(encrypted, sizeof(uint8_t), strlen(msg), fptr);
    fclose(fptr);
    // TODO: Add bitwise assertion
}