#include <stddef.h>
#include "include/embed.h"

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password);

uint8_t *
embed(uint8_t *carrierBmp, size_t carrierSize, const char *msgPath, size_t msgSize,
      UserInput userInput)
{
    uint8_t *outputBmp = NULL;

    uint8_t *msg;

    if (userInput.encryption != NONE)
        msg = encrypt(msg, userInput.encryption, userInput.mode, userInput.password);

    switch (userInput.stegoAlgorithm)
    {
    case LSB1:
        // TODO: Merge call to LSB1 implementation
        outputBmp = lsb1Embed(carrierBmp, msgPath);
        break;
    case LSB4:
        // TODO: Merge call to LSB4 implementation
        //            outputBmp = lsb4Embed(carrierBmp, msg);
        break;
    case LSBI:
        // TODO: Merge call to LSB1 implementation
        //            outputBmp = lsbiEmbed(carrierBmp, msg);
        break;
    }

    return outputBmp;
}

uint8_t *lsb1Embed(const uint8_t *carrierBmp, const char *msgPath)
{
    printf("%s\n", msgPath);
    ulong bytesNeeded = getBytesNeededToStego(msgPath, LSB1);
    printf("Despues de bytes needed\n");
    BMP_HEADER *bmpHeader = parseBmp("asd", carrierBmp);
    printf("bytes needed %ld\n",     bytesNeeded);
    return carrierBmp;
}

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}