#include <stddef.h>
#include "embed.h"

uint8_t *encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password);

uint8_t *
embed(uint8_t *carrierBmp, size_t carrierSize, const uint8_t *msg, size_t msgSize,
        UserInput userInput)
{
    uint8_t *outputBmp = NULL;

    if (userInput.encryption != NONE)
        msg = encrypt(msg, userInput.encryption, userInput.mode, userInput.password);

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            // TODO: Merge call to LSB1 implementation
//            outputBmp = lsb1Embed(carrierBmp, msg);
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

uint8_t *
encrypt(const uint8_t *msg, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}