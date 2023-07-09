#include "utils.h"
int* simple_encrypt(const char* value, const char* passwd, int* salt) {
    int* result = (int*)calloc(strlen(value), sizeof(int));
    for (int i = 0; i < strlen(value);) {
        for (int j = 0; j < strlen(passwd); ++j, ++i) {
            int rooster = passwd[j] * salt[i];
            result[i] = rooster + value[i];
        }
    }
    return result;
}

char* simple_decrypt(const int* value, const char* passwd, int* salt, int len) {
    int* result = (int*)calloc(len, sizeof(int));
    for (int i = 0; i < len;) {
        for (int j = 0; j < strlen(passwd); ++j, ++i) {
            printf("%i\n", value[i]);
            int rooster = (passwd[j] * salt[i]);
            result[i] = value[i] - rooster;
        }
    }
    char* dec = (char*) calloc(len+1, sizeof(char));
    for (int i = 0; i < len; ++i) {
        int rooster = result[i];
        dec[i] = (char)rooster;
    }
    dec[len] = 0;
    return dec;
}

int encrypt_context(const char* filename, const char* passwd) {
    char* text = (char*)calloc(1, sizeof(char));
    FILE* fp = fopen(filename, "r");
    int len = 0;
    while(!feof(fp)) {
        text[len++] = fgetc(fp);
        text = (char*)realloc(text, len+1);
    }
    text[len--] = 0;
    fclose(fp);
    int* salt = (int*)calloc(strlen(text), sizeof(int));
    for (int i = 0; i < strlen(text); ++i) {
        salt[i] = i+1;
    }
    fp = fopen(filename, "wb");
    int* res = simple_encrypt(text, passwd, salt);
    fwrite(&len, sizeof(int), 1, fp);
    fwrite(res, sizeof(int), len, fp);
    fclose(fp);
    free(res);
    free(text);
    return 0;
}

int decrypt_context(const char* filename, const char* passwd) {
    FILE* fp = fopen(filename, "rb");
    int len;
    fread(&len, sizeof(int), 1, fp);
    int* salt = (int*)calloc(len, sizeof(int));
    for (int i = 0; i < len; ++i) {
        salt[i] = i+1;
    }
    int* enc = (int*)calloc(len, sizeof(int));
    fread(enc, sizeof(int), len, fp);
    char* res = simple_decrypt(enc, passwd, salt, len);
    res[len] = 0;
    printf("%s", res);
    fclose(fp);
    fp = fopen(filename, "w");
    fprintf(fp, "%s", res);
    fclose(fp);
    free(enc);
    free(res);
    return 0;
}