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

#define PAGE_SZ64 0x1000
#define PAGE_SZ32 0x1000

#define OUTPUT_FILE_NAME "woody"
#define PAYLOAD_64_NAME "./payloads/payload_64"
#define PAYLOAD_32_NAME "./payloads/payload_32"

/* Custom types for 32bit compatibility. */

#ifdef ARCH_32
/* Data types for 32bit */
typedef Elf32_Addr t_elf_addr;    // (Size 4) Unsigned program addresses
typedef Elf32_Off t_elf_off;      // (Size 4) Unsigned file offsets
typedef Elf32_Half t_elf_half;    // (Size 2) Unsigned medium integers
typedef Elf32_Word t_elf_word;    // (Size 4) Unsigned large integers
typedef Elf32_Sword t_elf_sword;  // (Size 4) Signed large integers
typedef Elf32_Word t_elf_xword;   // Defined for compatibility with Elf64_Xword
typedef Elf32_Sword t_elf_sxword; // Defined for compatibility with Elf64_Sxword

/* Data structures for 32bit */
typedef Elf32_Ehdr t_elf_ehdr; // Elf header located at file offset 0
typedef Elf32_Phdr t_elf_phdr; // Program headers contaning segment informations
typedef Elf32_Shdr t_elf_shdr; // Section header containing section informations
typedef Elf32_Sym t_elf_sym;   // Symbol table entries
typedef Elf32_Rel t_elf_rel;   // Relocation entries
typedef Elf32_Rela t_elf_rela; // Relocation entries with an r_addend member
typedef Elf32_Dyn t_elf_dyn;   // Dynamic table entries

#else /* not WOODY_32BIT */

/* Data types for 64bit */
typedef Elf64_Addr t_elf_addr; // (Size 8) Unsigned program addresses
typedef Elf64_Ehdr t_elf_ehdr;
typedef Elf64_Phdr t_elf_phdr;
typedef Elf64_Shdr t_elf_shdr;
typedef Elf64_Off t_elf_off;

#endif /* not WOODY_32BIT */

#define size_t uint32_t

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_elf64_ptrs
{
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
} t_elf64_ptrs;

typedef struct s_elf32_ptrs
{
    Elf32_Ehdr *ehdr;
    Elf32_Phdr *phdr;
    Elf32_Shdr *shdr;
    Elf32_Addr new_entry_point;
    Elf32_Addr old_entry_point;

    Elf32_Off text_start_offset;
    Elf32_Off text_end_offset;

} t_elf32_ptrs;

typedef struct s_woody
{
    void *mmap_ptr;
    uint32_t binary_data_size;

    void *payload_data;
    uint32_t payload_size;

    void *cipher;

    char ei_class; //Used as a flag for elfclass.

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
    uint32_t infected_file_size;
} t_woody;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

void error(int err, t_woody *woody);
void free_woody(t_woody *woody);
void pt_note_to_pt_load_infection(t_woody *woody);
void cipher_woody_file_data(t_woody *woody);
void print_memory(void *memory_ptr, int memory_size);
void check_elf_header_and_set_type(t_woody *woody);

void infect_elf(t_woody *woody);
void silvio_text_infection_elf(t_woody *woody);

void set_elf64_ptr(t_woody *woody);
void load_payload(t_woody *woody, char *payload_name);
void set_woody_ptrs_to_null(t_woody *woody);

/****************************************************************************/
/*                          GLOBAL VARIABLES                                */
/****************************************************************************/

#endif
