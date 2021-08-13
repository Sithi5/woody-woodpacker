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

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

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

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

/* Colors */
#define END "\033[0m"
#define RED "\033[0;31m"
#define B_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define B_GREEN "\033[1;32m"
#define YELLOW "\033[0;33m"
#define B_YELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define B_BLUE "\033[1;34m"
#define MAGENTA "\033[0;35m"
#define B_MAGENTA "\033[1;35m"
#define CYAN "\033[0;36m"
#define B_CYAN "\033[1;36m"

/* Error codes */
enum e_error
{
    ERROR_OPEN = 1,
    ERROR_READ,
    ERROR_WRITE,
    ERROR_CLOSE,
    ERROR_MALLOC,
    ERROR_INPUT_ARGUMENTS_NUMBERS,
    ERROR_LSEEK,
    ERROR_NOT_ELF,
    ERROR_NOT_ELF32,
    ERROR_NOT_ELF64,
    ERROR_MMAP,
    ERROR_NOT_EXECUTABLE_BINARY,
    ERROR_ELF_NOT_LITTLE_ENDIAN,
    ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD,
    ERROR_RET2OEP_NOT_FOUND,
    ERROR_RET2TEXTSECTION_NOT_FOUND,
    ERROR_SETTEXTSECTIONSIZE_NOT_FOUND,
    ERROR_FILE_IS_ALREADY_INFECTED,
    NB_OF_ERROR_CODES /* Always keep last */
};

#define PAGE_SIZE 0x1000

#define OUTPUT_FILE_NAME "woody"

/* Custom types for 32bit compatibility. */
#ifdef ARCH_32
/* Data types for 32bit */
typedef Elf32_Addr t_elf_addr;
typedef Elf32_Ehdr t_elf_ehdr;
typedef Elf32_Phdr t_elf_phdr;
typedef Elf32_Shdr t_elf_shdr;
typedef Elf32_Off t_elf_off;
#define size_t uint32_t
#define PAYLOAD_NAME "./payloads/payload_32"

#else /* 64 bits */

// If ARCH_32 not define, define it to 0.
#define ARCH_32 0

/* Data types for 64bit */
typedef Elf64_Addr t_elf_addr; // (Size 8) Unsigned program addresses
typedef Elf64_Ehdr t_elf_ehdr;
typedef Elf64_Phdr t_elf_phdr;
typedef Elf64_Shdr t_elf_shdr;
typedef Elf64_Off t_elf_off;
#define size_t uint64_t
#define PAYLOAD_NAME "./payloads/payload_64"

#endif

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_woody
{
    void *mmap_ptr;
    size_t binary_data_size;

    void *payload_data;
    size_t payload_size;

    void *cipher;

    t_elf_ehdr *ehdr;
    t_elf_phdr *phdr;
    t_elf_shdr *shdr;
    t_elf_addr new_entry_point;
    t_elf_addr old_entry_point;
    t_elf_addr payload_vaddr;

    t_elf_off text_start_offset;
    t_elf_off text_end_offset;
    t_elf_off text_section_size;
    t_elf_addr text_p_vaddr;

    int ret2oep_offset;

    void *infected_file;
    size_t infected_file_size;
} t_woody;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

void error(int err, t_woody *woody);
void free_woody(t_woody *woody);
void pt_note_to_pt_load_infection(t_woody *woody);
void cipher_woody_file_data(t_woody *woody);
void print_memory(void *memory_ptr, size_t memory_size);
void check_elf_header(t_woody *woody);

void silvio_text_infection(t_woody *woody);

void load_payload(t_woody *woody, char *payload_name);
void set_woody_ptrs_to_null(t_woody *woody);

/****************************************************************************/
/*                          GLOBAL VARIABLES                                */
/****************************************************************************/

#endif
