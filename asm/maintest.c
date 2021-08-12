/* example.c */
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    extern char *rc4_cipher_start(char *data, int datalen, char *key, int keylen);
    char data[18] = "wakonda for ever\n";
    char *key = "KEY";
    void *crypt;
    char *decrypt;
    int len = strlen(data);
    int keylen = strlen(key);
    printf("data:%s | key: %s \n", data, key);
    rc4_cipher_start(data, len, key, keylen);
    // int cryptlen = strlen(crypt);
    // printf("crypt %s\n", crypt);
    // printf("hexvalue: |");
    for (int i = 0; i < 18; i++)
    {
        printf(" %x |", ((unsigned char *)(data))[i]);
    }
    // // printf("\n");
    // // printf("crypt lenght = %d\n\n", strlen(crypt));
    // decrypt = rc4_cipher_start(data, len, key, keylen);
    // int decryptlen = strlen(decrypt);
    // // printf("decrypt %s\n",decrypt);
    // // printf("hexvalue: |");
    // for (int i = 0; i < decryptlen; i++)
    // {
    //     printf(" %2x |", decrypt[i]);
    // }
    // printf("\n");
    // printf("decrypt lenght = %d\n", strlen(decrypt));
    // for (int i = 0; i < 18; i++)
    // {
    //     printf("%c", ((unsigned char *)(crypt))[i]);
    //     printf("%#x ", ((unsigned char *)(crypt))[i]);
    // }
}