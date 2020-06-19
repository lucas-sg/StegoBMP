#include "extract.h"

uint8_t *decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password);

uint8_t *
extract(uint8_t *carrierBmp, size_t carrierSize, UserInput userInput)
{
    uint8_t *output = NULL, *embeddedBytes = NULL;

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
            // TODO: Merge call to LSB1 implementation
//            embeddedBytes = lsb1Extract(carrierBmp, msg);
            break;
        case LSB4:
            // TODO: Merge call to LSB4 implementation
//            embeddedBytes = lsb4Extract(carrierBmp, msg);
            break;
        case LSBI:
            // TODO: Merge call to LSB1 implementation
//            embeddedBytes = lsbiExtract(carrierBmp, msg);
            break;
    }

    if (userInput.encryption != NONE)
        output = decrypt(embeddedBytes, userInput.encryption, userInput.mode, userInput.password);

    return output;
}

uint8_t *
decrypt(const uint8_t *encryptedBytes, ENCRYPTION encryption, ENC_MODE mode, const char *password)
{
    // TODO: Encrypt with OpenSSL API
}