/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:12:08 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:07:03 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void print_loop(int fd)
{
    char buf[11];
    int l;
    printf("fd = %d \n", fd);
    while ((l = read(fd, &buf, 10)) > 0)
    {
        buf[l] = '\0';
        printf("%s", buf);
    }
}

void get_file_data(t_woody *woody)
{
    if ((woody->file_data_len = lseek(woody->fd, 0, SEEK_END)) != -1)
    {
        printf("Input file size = %d\n", woody->file_data_len);
        /* Allocate our buffer to that size. */
        if (!(woody->file_data = malloc(sizeof(char) * (woody->file_data_len + 1))))
        {
            error(ERROR_MALLOC, woody);
        }

        /* Go back to the start of the file. */
        if (lseek(woody->fd, 0, SEEK_SET) != 0)
        {
            error(ERROR_LSEEK, woody);
        }

        /* Read the entire file */
        if (read(woody->fd, woody->file_data, woody->file_data_len) == -1)
        {
            error(ERROR_READ, woody);
        }
        woody->file_data[woody->file_data_len] = '\0';
    }
    else
    {
        error(ERROR_LSEEK, woody);
    }
}

/* Simple XOR encryption/decryption algorithm. */
char *XORCipher(char *data, char *key, int data_len, int key_len)
{
    char *output = (char *)malloc(sizeof(char) * data_len);

    for (int i = 0; i < data_len; ++i)
    {
        output[i] = data[i] ^ key[i % key_len];
    }

    return output;
}

void cipher_woody_file_data(t_woody *woody)
{
    woody->cipher = XORCipher(woody->file_data, "MyXorKey", woody->file_data_len, 5);
    printf("Woody cipher = %s\n", woody->cipher);
}

void write_woody_file(t_woody *woody)
{
    int fd;
    if ((fd = open("woody", O_WRONLY | O_CREAT, 0777)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if ((write(fd, woody->file_data, woody->file_data_len)) == -1)
    {
        error(ERROR_WRITE, woody);
    }
    if ((close(fd)) == -1)
    {
        error(ERROR_CLOSE, woody);
    }
}

int main(int ac, char **av)
{
    t_woody *woody;

    if (!(woody = (t_woody *)malloc(sizeof(t_woody))))
        error(ERROR_MALLOC, woody);
    if (ac != 2)
    {
        error(ERROR_INPUT_ARGUMENTS_NUMBERS, woody);
    }

    if ((woody->fd = open(av[1], O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    woody->file_name = av[1];
    get_file_data(woody);
    if ((close(woody->fd)) == -1)
    {
        error(ERROR_CLOSE, woody);
    }
    cipher_woody_file_data(woody);
    write_woody_file(woody);
    free_woody(woody);
    return 0;
}
