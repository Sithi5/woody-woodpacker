/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:12:08 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 09:35:57 by mabouce          ###   ########.fr       */
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
    int buff_size;
    if ((buff_size = lseek(woody->fd, 0, SEEK_END)) != -1)
    {
        printf("Input file size = %d\n", buff_size);
        /* Allocate our buffer to that size. */
        if (!(woody->file_data = malloc(sizeof(char) * (buff_size + 1))))
        {
            error(ERROR_MALLOC, woody);
        }

        /* Go back to the start of the file. */
        if (lseek(woody->fd, 0, SEEK_SET) != 0)
        {
            error(ERROR_LSEEK, woody);
        }

        /* Read the entire file */
        read(woody->fd, woody->file_data, buff_size);
        woody->file_data[buff_size] = "\0";
    }
    else
    {
        error(ERROR_LSEEK, woody);
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
    printf("Woody file data = %s", woody->file_data);
    if ((close(woody->fd)) == -1)
    {
        error(ERROR_CLOSE, woody);
    }
    return 0;
}
