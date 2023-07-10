#include "utils.h"

///Return value: decrypted string if succeeded, NULL otherwise;
int* gen_salt(int seed, int len) {
    srand(seed);//set the seed to the value provided by the user
    int* salt = (int*)calloc(len, sizeof(int));//allocate the memory for salt
    if (!salt) {
        return NULL;
    }//check if it worked
    for (int i = 0; i < len; ++i) {
        salt[i] = rand() % 255 + 1;//fill the array with pseudo-random numbers
    }
    return salt;
}

///Return value: decrypted string if succeeded, NULL otherwise;
int* simple_encrypt(const char* value, const char* passwd, int* salt) {
    int* result = (int*)calloc(strlen(value), sizeof(int));
    if (!result) {
        return NULL;
    }//allocate an array of ints for keeping an encrypted data
    for (int i = 0; i < strlen(value);) {
        for (int j = 0; j < strlen(passwd); ++j, ++i) {
            int rooster = passwd[j] * salt[i];
            result[i] = rooster + value[i];//add some password-based number to each character of input array
        }
    }
    return result;
}

///Return value: decrypted string if succeeded, NULL otherwise;
char* simple_decrypt(const int* value, const char* passwd, int* salt, int len) {
    int* result = (int*)calloc(len, sizeof(int));
    if (!result) {
        return NULL;
    }
    for (int i = 0; i < len;) {
        for (int j = 0; j < strlen(passwd); ++j, ++i) {
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
    return dec;//the same as encryption but we convert integers back to chars and allocate an array for them
}

///Return values: 0 for success, 1 for memory allocation failure, 2 for file not existing, 3 for file creation error;
int encrypt_context(const char* filename, const char* passwd, const char* seed) {
    char* text = (char*)calloc(1, sizeof(char));//allocate memory for a text
    if (!text) {
        return 1;
    }
    FILE* fp = fopen(filename, "r");//open a file for reading
    if (!fp) {
        free(text);
        return 2;
    }
    int len = 0;
    while(!feof(fp)) {
        text[len++] = fgetc(fp);
        char* newarr = (char*)realloc(text, len+1);
        if (!newarr) {
            free(text);
            fclose(fp);
            return 1;
        }//doing some tricks because of how realloc works
        text = newarr;//using cppstring was tempting but I think that would be an overkill for the things I am about to do
    }
    text[len--] = 0;//null-terminate string
    fclose(fp);//closing a file
    int s = atoi(seed);
    int* salt = gen_salt(s, len);
    if (!salt) {
        free(text);
        return 1;
    }//generate salt
    fp = fopen(filename, "wb");//open file for writing data in binary
    if (!fp) {
        free(text);
        free(salt);
        return 3;
    }

    int* res = simple_encrypt(text, passwd, salt);
    if (!res) {
        fclose(fp);
        free(text);
        free(salt);
        return 1;
    }//encrypt the data
    fwrite(&len, sizeof(int), 1, fp);
    fwrite(res, sizeof(int), len, fp);//write data and length(it would be same as text but I do not have a degree in cybersecurity so idc)
    fclose(fp);
    free(res);
    free(text);
    return 0;
}

///Return values: 0 for success, 1 for memory allocation failure, 2 for file not existing, 3 for file creation error, 4 for file being corrupted;
int decrypt_context(const char* filename, const char* passwd, const char* seed) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        return 2;
    }//open the file
    int len;
    int err = fread(&len, sizeof(int), 1, fp);
    if (err != 1) {
        fclose(fp);
        return 4;
    }//read length

    int s = atoi(seed);
    int* salt = gen_salt(s, len);
    if (!salt) {
        fclose(fp);
        return 1;
    }//generate salt
    int* enc = (int*)calloc(len, sizeof(int));//allocate memory for encrypted string
    if (!enc) {
        free(salt);
        fclose(fp);
        return 1;
    }
    err = fread(enc, sizeof(int), len, fp);
    if (err != len) {
        free(enc);
        free(salt);
        fclose(fp);
        return 4;
    }//read data, check for reading errors

    char* res = simple_decrypt(enc, passwd, salt, len);
    if (!res) {
        free(enc);
        free(salt);
        fclose(fp);
        return 1;
    }//decrypt a message
    res[len] = 0;
    fclose(fp);//close file
    fp = fopen(filename, "w");//open for writing
    if (!fp) {
        free(enc);
        free(res);
        return 3;
    }
    fprintf(fp, "%s", res);
    fclose(fp);//write, close file
    free(enc);
    free(res);
    return 0;
}