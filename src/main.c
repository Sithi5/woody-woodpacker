/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:12:08 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:18:26 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void get_file_data(t_woody *woody)
{
    if ((woody->file_data_len = lseek(woody->fd, 0, SEEK_END)) != -1)
    {
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

void write_woody_file(t_woody *woody)
{
    int fd;
    if ((fd = open("woody", O_WRONLY | O_CREAT, S_IRWXU)) == -1)
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
