#include "woody_woodpacker.h"

void random_string(int len, t_woody *woody)
{
    char key[KEY_LEN];
    int fd;

    if ((fd = open("/dev/random", O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if (read(fd, key, len) == -1)
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_READ, woody);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
    memcpy((void *)woody->encryption_key, key, len);
    printf("KEY IN HEX: %lu\n", strlen(woody->encryption_key));
    for (int n = 0; n < KEY_LEN; n++)
    {
        printf("%x |", ((char *)woody->encryption_key)[n]);
    }
    printf("\n");
}

void key_generator(t_woody *woody)
{
    if (!(woody->encryption_key = (char *)malloc(sizeof(char) * KEY_LEN + 1)))
        error(ERROR_MALLOC, woody);
    random_string(KEY_LEN, woody);
    printf("key %s\n", woody->encryption_key);
}