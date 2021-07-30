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

/*
** includes
*/

#include <stdio.h>       // FFLUSH FPUTS PERROR
#include <stdlib.h>      // MALOC/FREE
#include <fcntl.h>       // OPEN
#include <sys/syscall.h> // SYSCALL
#include <string.h>      // STRERROR
#include <sys/mman.h>    // MMAP MUNMAP
#include <unistd.h>      // CLOSE EXIT LSEEK WRITE
#include <elf.h>         // ELF STRUCTURE
#include <errno.h>
#include <stdbool.h>

/*
** Defines
*/

#define OUTPUT_FILE_NAME "woody"
#define PAYLOAD_NAME "./payload/woody_payload_test"

/*
** struct
*/

typedef struct s_woody
{
    long unsigned int payload_size;
    long unsigned int binary_data_size;

    void *mmap_ptr;
    void *payload_data;

    Elf64_Ehdr *ehdr;
    Elf64_Phdr *phdr;
    Elf64_Shdr *shdr;
    Elf64_Addr new_entry_point;
    Elf64_Addr old_entry_point;

    bool is_exec;
    bool is_dyn;

} t_woody;

/*
** Functions definitions
*/

void error(int err, t_woody *woody);
void free_woody(t_woody *woody);
void check_ehdr(t_woody *woody);
void elf64_pt_note_to_pt_load_infection(t_woody *woody);
void silvio_text_infection(t_woody *woody);
/*
** ERROR CODE
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
#define ERROR_NOT_EXECUTABLE_BINARY 10
#define ERROR_ELF_NOT_LITTLE_ENDIAN 11
#define ERROR_FILE_SIZE_TOO_SMALL 12

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

#endif
