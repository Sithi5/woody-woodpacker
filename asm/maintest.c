/* example.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h> // CLOSE EXIT LSEEK WRITE
#include <stdlib.h> // MALOC/FREE
#include <fcntl.h>  // OPEN

#define N 256

void ft_swap(unsigned char *a, unsigned char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void encryption(unsigned char *stream, char *result, char *data, int len)
{
    int i = 0, j = 0, res = 0;

    for (int k = 0; k < len; k++)
    {
        i = (i + 1) % N;
        j = (j + stream[i]) % N;
        ft_swap(&stream[i], &stream[j]);
        res = stream[(stream[i] + stream[j]) % N];
        result[k] = res ^ data[k];
    }
}

void Keystream_generation(unsigned char *stream, char *key, int key_len)
{
    int j = 0;
    for (int i = 0; i < N; i++)
    {
        stream[i] = i;
    }
    for (int i = 0; i < N; i++)
    {
        j = (j + stream[i] + key[i % key_len]) % N;
        ft_swap(&stream[i], &stream[j]);
    }
}

int main(int argc, char *argv[])
{
    extern char *rc4_cipher_start(char *data, int datalen, char *key, int keylen);
    extern char *rc4_encrypt(char *data, int datalen, char *key, int keylen);
    char data[18] = "wakonda for ever\n";
    char data2[18] = "wakonda for ever\n";
    char *key = "KEY";
    unsigned char *stream;
    unsigned char *result;
    int len = strlen(data);
    int keylen = strlen(key);
    printf("data:%s| key: %s \n", data, key);
    rc4_cipher_start(data, len, key, keylen);
    if (!(stream = (unsigned char *)malloc(sizeof(unsigned char) * N)))
        return 0;
    if (!(result = (unsigned char *)malloc(sizeof(unsigned char) * len)))
        return 0;
    Keystream_generation(stream, key, keylen);
    encryption(stream, result, data2, len);
    //rc4_encrypt(data, len, key, keylen);
    // int cryptlen = strlen(crypt);
    // printf("crypt %s\n", crypt);
    printf("| hexvalue: |\n");
    // for (int i = 0; i < 18; i++)
    // {
    //     printf(" %x |", ((unsigned char *)(data))[i]);
    // }
    for (int i = 0; i < 18; i++)
    {
        printf(" %x |", ((unsigned char *)(data))[i]);
    }
    printf("\n text crypt in c \n");
    for (int i = 0; i < 18; i++)
    {
        printf(" %x |", ((unsigned char *)(result))[i]);
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