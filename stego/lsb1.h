#ifndef LSB1_H
#define LSB1_H

/**

**/
int *lsb1(const int *bmpFile, const char *cipherText);
int replaceLSB(const int bmpByte, const char cipherTextByte, int cbCursor);

#endif