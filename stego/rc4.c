#include "../include/rc4.h"
#include <string.h>

uint8_t *calculateKey(const uint8_t *bmpFile);
uint8_t *KSA(const uint8_t *key);
uint8_t *initVectorS();
uint8_t *initVectorT(const uint8_t *key);
void swap(uint8_t *s, uint32_t i, uint32_t j);
uint8_t *PRGA(uint8_t *s, size_t len);

static const size_t VECTOR_SIZE = 256;

uint8_t * RC4(const uint8_t *msg, const uint8_t *bmpFile, const size_t msgSize)
{
    uint8_t *key = calculateKey(bmpFile);
    uint8_t *s = KSA(key);
    uint8_t *keyStream = PRGA(s, msgSize);
    uint8_t *encryptedMsg = malloc(sizeof(*encryptedMsg) * msgSize);

    for (size_t i = 0; i < msgSize; i++)
    {
        encryptedMsg[i] = msg[i] ^ keyStream[i];
    }

    free(key);
    free(s);
    free(keyStream);

    return encryptedMsg;
}

uint8_t * calculateKey(const uint8_t *bmpFile)
{
    uint8_t *key = malloc(sizeof(*key) * KEY_SIZE);

    memcpy(key, bmpFile, KEY_SIZE);

    return key;
}

uint8_t * KSA(const uint8_t *key)
{
    uint8_t *s = initVectorS();
    uint8_t *t = initVectorT(key);

    for (uint32_t i = 0, j = 0; i < VECTOR_SIZE; i++)
    {
        j = (j + s[i] + t[i]) % VECTOR_SIZE;
        swap(s, i, j);
    }

    free(t);

    return s;
}

uint8_t * initVectorS()
{
    uint8_t *s = malloc(sizeof(*s) * VECTOR_SIZE);

    for (size_t i = 0; i < VECTOR_SIZE; i++)
        s[i] = (uint8_t)i;

    return s;
}

uint8_t * initVectorT(const uint8_t *key)
{
    uint8_t *t = malloc(sizeof(*t) * VECTOR_SIZE);

    for (size_t i = 0; i < VECTOR_SIZE; i++)
        t[i] = key[i % KEY_SIZE];

    return t;
}

uint8_t * PRGA(uint8_t *s, const size_t len)
{
    uint32_t i = 0, j = 0, k = 0, t;
    uint8_t *keyStream = malloc(sizeof(*keyStream) * len);

    while (k < len)
    {
        i = (i + 1) % VECTOR_SIZE;
        j = (j + s[i]) % VECTOR_SIZE;
        swap(s, i, j);
        t = (s[i] + s[j]) % VECTOR_SIZE;
        keyStream[k++] = s[t];
    }

    return keyStream;
}

void swap(uint8_t *s, const uint32_t i, const uint32_t j)
{
    uint8_t aux;
    aux = s[i];
    s[i] = s[j];
    s[j] = aux;
}
