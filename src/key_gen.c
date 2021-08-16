#include "woody_woodpacker.h"

void random_string(int len, t_woody *woody)
{
    char key[128];
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
    memcpy(woody->encryption_key, key, len);
}

void key_generator(t_woody *woody) {
    if (!(woody->encryption_key = (char *)malloc(sizeof(char) * 128)))
        error(ERROR_MALLOC, woody);
    random_string(128,woody);
}