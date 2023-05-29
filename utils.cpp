#include "utils.h"

void clearBuffer(){
    if(getchar()!='\n') clearBuffer();
}

void handleErrors()
{

}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

char* PassToVector(char* pass) {
    char* vector = (char*)malloc(128);
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 16&&i < 128; ++j, ++i) {
            vector[i] = pass[j];
        }
    }
    return vector;
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int console_encrypt() {
    char password[129];
    std::string input;
    char got;
    printf("GIMME TEXT");
    while(true) {
        got = getchar();
        if (got == '\n') {
            break;
        }
        input.push_back(got);
    }
    printf("Now the passworde");
    scanf("%128[^\n]", password);
    clearBuffer();
    char* iv = PassToVector(password);
    char* cyphertext = (char *)malloc(strlen(password)*input.length());
    int len = encrypt((unsigned char*)input.data(), input.length(), (unsigned char*)password, (unsigned char*) iv, (unsigned char*) cyphertext);
    FILE* fp = fopen("encrypted.txt", "wb");
    fwrite(&len, sizeof(len), 1, fp);
    fwrite(cyphertext, sizeof(char), len, fp);
    fclose(fp);
    //free(cyphertext);
    return 0;
}

int console_decrypt() {
    char password[129];
    std::string input;
    char got;
    FILE* fp = fopen("encrypted.txt", "rb");
    int len;
    fread(&len, sizeof(len), 1, fp);
    int i = 0;
    while(i < len) {
        got = fgetc(fp);
        if (got == '\0'||feof(fp)) {
            break;
        }
        i++;
        input.push_back(got);
    }
    fclose(fp);
    printf("Now the passworde");
    scanf("%128[^\n]", password);
    clearBuffer();
    char* iv = PassToVector(password);
    char* plaintext = (char *)malloc(strlen(password)*input.length());
    int nlen = decrypt((unsigned char*)input.data(), input.length(), (unsigned char*)password, (unsigned char*) iv, (unsigned char*) plaintext);
    plaintext[nlen] = '\0';
    printf("%s", plaintext);
    free(plaintext);
    return 0;
}