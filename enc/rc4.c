#include <stdlib.h>


char *KSA(const char *key);
char *initVectorS();
char *initVectorT(const char *key);
void swap(char *s, int i, int j);
char *PRGA(char *s, size_t len);

size_t VECTOR_SIZE = 256;


char *RC4(const char *msg, const char *key) {
    size_t len         = strlen(msg);
    char *s            = KSA(key);
    char *keyStream    = PRGA(s, len);
    char *encryptedMsg = malloc(sizeof *encryptedMsg * len);

    for (int i = 0; i < len; i++) {
        encryptedMsg[i] = msg[i] ^ keyStream[i]; 
    }

    free(s);
    free(keyStream);

    return encryptedMsg;
}

char *KSA(const char *key) {
    char *s = initVectorS();
    char *t = initVectorT(key);

    for (int i = 0, j = 0; i < VECTOR_SIZE; i++) {
        j = (j + s[i] + t[i]) % VECTOR_SIZE;
        swap(s, i, j);
    }

    free(t);

    return s;
}

char *initVectorS() {
    char *s = malloc(sizeof *s *VECTOR_SIZE);

    for (int i = 0; i < VECTOR_SIZE; i++) {
        s[i] = i & 0xFF;
    }

    return s;
}

char *initVectorT(const char *key) {
    char *t = malloc(sizeof *t * VECTOR_SIZE);

    for (int i = 0; i < VECTOR_SIZE; i++) {
        t[i] = key[i % strlen(key)];
    }

    return t;
}

char *PRGA(char *s, size_t len) {
    int i = 0, j = 0, k = 0, t = 0;
    char *keyStream = malloc(sizeof *keyStream * len);

    while (k < len) {
        i = (i + 1) % VECTOR_SIZE;
        j = (j + s[i]) % VECTOR_SIZE;
        swap(s, i, j);
        t = (s[i] + s[j]) % VECTOR_SIZE;
        keyStream[k++] = s[t];
    }
}

void swap(char *s, int i, int j) {
    if (i < 0 || j < 0 || i >= strlen(s) || j >= strlen(s))
        return; // TODO: Set error

    char aux = s[i];
    s[i]     = s[j];
    s[j]     = aux;
}
