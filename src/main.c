/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:12:08 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:48 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void get_file_data(char *file_name, t_woody *woody)
{
    if ((woody->fd = open(file_name, O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if ((woody->old_binary_data_len = lseek(woody->fd, 0, SEEK_END)) != -1)
    {
        /*
        ** TODO remove next line
        */
        woody->new_binary_data_len = woody->old_binary_data_len;
        /* Go back to the start of the file. */
        if (lseek(woody->fd, 0, SEEK_SET) != 0)
        {
            close(woody->fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
        }
        /* Copy binary address map*/
        if (!(woody->mmap_ptr = mmap(0, woody->old_binary_data_len, PROT_READ, MAP_PRIVATE, woody->fd, 0)))
        {
            close(woody->fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_MMAP, woody);
        }
    }
    else
    {
        close(woody->fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
    }
    close(woody->fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
}

void write_woody_file(t_woody *woody)
{
    int fd;

    if ((fd = open(OUTPUT_FILE_NAME, O_WRONLY | O_CREAT, S_IRWXU)) < 0)
    {
        error(ERROR_OPEN, woody);
    }
    if ((write(fd, woody->mmap_ptr, woody->new_binary_data_len)) < 0)
    {
        if ((close(fd)) < 0)
        {
            error(ERROR_CLOSE, woody);
        }
        error(ERROR_WRITE, woody);
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
    woody->is_exec = true;
    woody->is_dyn = true;

    get_file_data(av[1], woody);
    check_elf_header(woody);
    elf64_pt_note_to_pt_load_infection(woody);
    write_woody_file(woody);
    free_woody(woody);
    return 0;
}
