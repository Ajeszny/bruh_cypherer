#ifndef ENCRYPT_UTILS_H
#define ENCRYPT_UTILS_H
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <cstdio>
#include <cmath>
#include <openssl/pem.h>
#include <cstring>
#include <string>
#include <cstdlib>
int decrypt_context(const char* filename, const char* passwd);
int encrypt_context(const char* filename, const char* passwd);
char* simple_decrypt(const int* value, const char* passwd, int* salt, int len);
int* simple_encrypt(const char* value, const char* passwd, int* salt);
#endif //ENCRYPT_UTILS_H
