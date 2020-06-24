#include "embed.h"
#include "cryptoUtils.h"
#include "stego/lsbEncrypt.h"
#include <string.h>


size_t buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer);


uint8_t *
embed(uint8_t *carrierBmp, size_t carrierLen, const uint8_t *plaintext, size_t ptextLen, const char *fileExtension,
      UserInput userInput)
{
    uint8_t *outputBmp     = NULL;
    // TODO: Change this to allocate size for ptextLen + plaintext + fileExtension
    uint8_t *inputSequence = NULL;
    size_t inputSeqLen     = buildInputSequence(plaintext, ptextLen, fileExtension, inputSequence);
    uint8_t *dataToEmbed;
    size_t dataLen;

    if (userInput.encryption != NONE)
    {
        dataToEmbed = malloc((ptextLen/16 + 1) * 16);
        dataLen     = encrypt(inputSequence, inputSeqLen, dataToEmbed, userInput.encryption, userInput.mode,
                              userInput.password);
    }
    else
    {
        dataLen     = inputSeqLen;
        dataToEmbed = inputSequence;
    }

    switch (userInput.stegoAlgorithm)
    {
        case LSB1:
//            outputBmp = lsb1(carrierBmp, dataToEmbed, carrierLen, dataLen);
            break;
        case LSB4:
//            outputBmp = lsb4(carrierBmp, dataToEmbed, carrierLen, dataLen);
            break;
        case LSBI:
            // TODO: Merge call to LSBI implementation
//            outputBmp = lsbiEmbed(carrierBmp, msg);
            break;
    }

    return outputBmp;
}

int
encrypt(const uint8_t *plaintext, int ptextLen, uint8_t *ciphertext, ENCRYPTION encryption, ENC_MODE mode,
        const uint8_t *password)
{
    EVP_CIPHER_CTX *ctx;
    int auxLen, ciphertextLen;
    const EVP_CIPHER *cipher = determineCipherAndMode(encryption, mode);
    size_t keyLen = determineKeyLength(encryption);
    uint8_t *key  = malloc(keyLen);
    uint8_t *iv   = malloc(keyLen);
    EVP_BytesToKey(cipher, EVP_sha256(), NULL, password, (int)strlen((char *)password), 1, key, iv);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        failedToCreateCipherContext();

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        failedToInitCipherContext();

    if (EVP_EncryptUpdate(ctx, ciphertext, &auxLen, plaintext, ptextLen) != 1)
        failedToEncrypt();

    ciphertextLen = auxLen;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + auxLen, &auxLen) != 1)
        failedToFinalizeEnc();

    ciphertextLen += auxLen;
    EVP_CIPHER_CTX_free(ctx);

    return ciphertextLen;
}

size_t
buildInputSequence(const uint8_t *data, size_t size, const char *fileExtension, uint8_t *inputSequenceBuffer)
{
    // TODO: @Stu
}