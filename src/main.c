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

void get_binary_data(char *file_name, t_woody *woody)
{
    double binary_data_size;
    int fd;

    if ((fd = open(file_name, O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if ((binary_data_size = lseek(fd, 0, SEEK_END)) != -1)
    {
        woody->binary_data_size = (uint32_t)binary_data_size;
        /* Go back to the start of the file. */
        if (lseek(fd, 0, SEEK_SET) != 0)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
        }
        /* Copy binary address map*/
        if (!(woody->mmap_ptr = mmap(0, woody->binary_data_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)))
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_MMAP, woody);
        }
    }
    else
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
}

void write_woody_file(t_woody *woody)
{
    int fd;

    if ((fd = open(OUTPUT_FILE_NAME, O_WRONLY | O_CREAT, S_IRWXU)) < 0)
    {
        error(ERROR_OPEN, woody);
    }
    if ((write(fd, woody->infected_file, woody->infected_file_size)) < 0)
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_WRITE, woody);
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

    get_binary_data(av[1], woody);
    check_elf_type(woody);
    woody->ei_class == ELFCLASS64 ? infect_elf_64(woody) : infect_elf_32(woody);

    // check_ehdr(woody);
    silvio_text_infection(woody);
    write_woody_file(woody);
    // free_woody(woody);
    return 0;
}
