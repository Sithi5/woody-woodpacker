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