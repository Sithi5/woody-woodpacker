/* example.c */
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    extern char *asmxorcipher(char *data, char *key, int datalen, int keylen);
    char data[18] = "wakonda for ever\n";
    char *key = "KEY";
    char *crypt;
    char *decrypt;
    int len = strlen(data);
    int keylen = strlen(key);
    // printf("data:%s | key: %s \n", data,key);
    crypt = asmxorcipher(data, key, len, keylen);
    int cryptlen = strlen(crypt);
    // printf("crypt %s\n",crypt);
    // printf("hexvalue: |");
    for (int i = 0; i < cryptlen; i++)
    {
        // printf(" %2x |", crypt[i]);
    }
    // printf("\n");
    // printf("crypt lenght = %d\n\n", strlen(crypt));
    decrypt = asmxorcipher(crypt, key, len, keylen);
    int decryptlen = strlen(decrypt);
    // printf("decrypt %s\n",decrypt);
    // printf("hexvalue: |");
    for (int i = 0; i < decryptlen; i++)
    {
        printf(" %2x |", decrypt[i]);
    }
    // printf("\n");
    // printf("decrypt lenght = %d\n", strlen(decrypt));
}