#include "woody_woodpacker.h"

void random_string(t_woody *woody)
{
    char key[KEY_LEN];
    int fd;

    if ((fd = open("/dev/random", O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if (read(fd, key, KEY_LEN) == -1)
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_READ, woody);
    }
    // Convert random char to readable
    for (int index = 0; index < KEY_LEN; index++)
    {
        key[index] < 0 ? key[index] *= -1 : 0;
        key[index] = (key[index] % 125);
        key[index] < 49 ? key[index] += 49 : 0;
        printf("value of c = %i\n", key[index]);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
    memcpy((void *)woody->encryption_key, key, KEY_LEN);
    woody->encryption_key[KEY_LEN] = '\0';
    // printf("KEY IN HEX: %lu\n", strlen(woody->encryption_key));
    // for (int n = 0; n < KEY_LEN; n++)
    // {
    //     printf("%x |", ((char *)woody->encryption_key)[n]);
    // }
    // printf("\n");
}

void print_key(t_woody *woody)
{
    printf("key :\n");
    printf("%s\n", woody->encryption_key);
}

void key_generator(t_woody *woody)
{
    if (!(woody->encryption_key = (char *)malloc(sizeof(char) * KEY_LEN + 1)))
        error(ERROR_MALLOC, woody);
    random_string(woody);
    print_key(woody);
}

// S>R*>;2@3ys<0F);0UqFh!(ntjh]T^;H#/56UF.gT\&I_'<>,8cT;tZ,UrL[RXKA+P=N6h;6xa{q@LZf?Nv-Di'((MQK.<1'B)7a+_wu9U--]Lr3LM=SwJ=:ax
// 31r[8;8L2Z#e+9o<,2sn7\0g<H=+.8#k%3BL;jo7Cs06$5T&)/2]X/N1*'TqG@1<%7i%z"2%W{BxQ>Yg,+LIN9W=#J?49"+Yb6#*Q\)74c[w<1.rk)<1|.O<c!398D>