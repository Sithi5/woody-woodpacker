/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:22:18 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 18:43:00 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_WOODPACKER_H
#define WOODY_WOODPACKER_H

#include <stdio.h>       // FFLUSH FPUTS PERROR
#include <stdlib.h>      // MALOC/FREE
#include <fcntl.h>       // OPEN
#include <sys/syscall.h> // SYSCALL
#include <string.h>      // STRERROR
#include <sys/mman.h>    // MMAP MUNMAP
#include <unistd.h>      // CLOSE EXIT LSEEK WRITE
#include <elf.h>         // ELF STRUCTURE
#include <errno.h>

typedef struct s_woody
{
    int fd;
    int file_data_len;
    int cipher_len;
    unsigned long int *addr_entrypoint;

    const char *file_name;
    char *cipher;
    char *architecture;

    void *mmap_ptr;
} t_woody;

/*
** ERROR
*/

#define ERROR_OPEN 1
#define ERROR_READ 2
#define ERROR_WRITE 3
#define ERROR_CLOSE 4
#define ERROR_MALLOC 5
#define ERROR_INPUT_ARGUMENTS_NUMBERS 6
#define ERROR_LSEEK 7
#define ERROR_NOT_ELF64 8
#define ERROR_MMAP 9
#define ERROR_MUNMAP 10

/*
** COLOR
*/

#define DEF "\033[0m"
#define RED "\033[0;31m"
#define B_RED "\033[1;31m"
#define GRE "\033[0;32m"
#define B_GRE "\033[1;32m"
#define YEL "\033[0;33m"
#define B_YEL "\033[1;33m"
#define BLU "\033[0;34m"
#define B_BLU "\033[1;34m"
#define MAG "\033[0;35m"
#define B_MAG "\033[1;35m"
#define CYA "\033[0;36m"
#define B_CYA "\033[1;36m"

void error(int err, t_woody *woody);
void free_woody(t_woody *woody);
void cipher_woody_file_data(t_woody *woody);
void check_binary_architecture(t_woody *woody);

#endif
