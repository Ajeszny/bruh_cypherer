#ifndef ENCRYPT_UTILS_H
#define ENCRYPT_UTILS_H
#include <openssl/rsa.h>
#include <cstdio>
#include <cmath>
#include <openssl/pem.h>
#include <string>
#include <cstdlib>
char* PassToVector(char* pass);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);
void handleErrors();
int console_encrypt();
int console_decrypt();
#endif //ENCRYPT_UTILS_H
