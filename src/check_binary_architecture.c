/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_binary_architecture.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:29:10 by maginist          #+#    #+#             */
/*   Updated: 2021/07/27 18:40:56 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void check_binary_architecture(t_woody *woody)
{
    Elf64_Ehdr *header;

    header = (Elf64_Ehdr *)woody->mmap_ptr;
    if (!(header->e_ident[1] == ELFMAG1 &&
          header->e_ident[2] == ELFMAG2 &&
          header->e_ident[3] == ELFMAG3 && /*Identify executable and ELF*/
          header->e_ident[EI_CLASS] == ELFCLASS64 &&
          header->e_ident[EI_VERSION] == EV_CURRENT &&
          header->e_ident[EI_OSABI] == ELFOSABI_SYSV &&
          header->e_ident[EI_ABIVERSION] == 0 &&
          header->e_ident[EI_PAD] == 0)) /*architecture 64 bits*/
    {
        error(ERROR_NOT_ELF64, woody);
    }
    if (!header->e_ident[EI_DATA] == ELFDATA2LSB)
    {
        error(ERROR_ELF_NOT_LITTLE_ENDIAN, woody);
    }
    if (header->e_type == ET_EXEC)
        woody->is_dyn = false;
    else if (header->e_type == ET_DYN)
    {
        woody->is_exec = false;
    }
    else
    {
        error(ERROR_NOT_EXECUTABLE_BINARY, woody);
    }
}

// The first check we make to the Ehdr is whether it is an Elf64 file.
// This means that the first four bytes (e_ident[EI_MAG0..EI_MAG3])
// must equal 'ELF64_MAGIC' ("\x7FELF").
// Next, we want e_ident[EI_CLASS] to be ELFCLASS64 (64-bit object)
// For e_ident[EI_VERSION] we want EV_CURRENT.
// For e_ident[EI_OSABI] we want ELFOSABI_SYSV and
// for e_ident[EI_ABIVERSION] we want 0 (zero).
// The seven bytes at e_ident[EI_PAD] should be zero.

// e_ident[EI_DATA] to equal ELFDATA2LSB (little-endian data structures).

// Next check if the EIF type is an executable or a shared library e_type == ET_EXEC or ET_DYN
