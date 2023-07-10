#ifndef ENCRYPT_UTILS_H
#define ENCRYPT_UTILS_H
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdlib>
// Function to decrypt an encrypted file given its name, password and seed
int decrypt_context(const char* filename, const char* passwd, const char* seed);

// Function to encrypt an encrypted file given its name, password and seed
int encrypt_context(const char* filename, const char* passwd, const char* seed);

// Functions contsining actual encryption/decryption algorythm(really simple as I'm too lazy to implement something super-secure, maybe later)
char* simple_decrypt(const int* value, const char* passwd, int* salt, int len);
int* simple_encrypt(const char* value, const char* passwd, int* salt);
#endif //ENCRYPT_UTILS_H
