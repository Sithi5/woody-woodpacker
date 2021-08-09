/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_elf64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 15:58:26 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/28 15:58:26 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void check_ehdr(t_woody *woody)
{

    /*
    The first check we make to the Ehdr is whether it is an Elf64 file.
    This means that the first four bytes (e_ident[EI_MAG0..EI_MAG3])
    must equal 'ELF64_MAGIC' ("\x7FELF").
    Next, we want e_ident[EI_CLASS] to be ELFCLASS64 (64-bit object)
    For e_ident[EI_VERSION] we want EV_CURRENT.
    For e_ident[EI_OSABI] we want ELFOSABI_SYSV and
    for e_ident[EI_ABIVERSION] we want 0 (zero).
    The seven bytes at e_ident[EI_PAD] should be zero.
    */
    if (!(woody->ehdr->e_ident[1] == ELFMAG1 &&
          woody->ehdr->e_ident[2] == ELFMAG2 &&
          woody->ehdr->e_ident[3] == ELFMAG3 &&
          woody->ehdr->e_ident[EI_CLASS] == ELFCLASS64 &&
          woody->ehdr->e_ident[EI_VERSION] == EV_CURRENT &&
          woody->ehdr->e_ident[EI_OSABI] == ELFOSABI_SYSV &&
          woody->ehdr->e_ident[EI_ABIVERSION] == 0 &&
          woody->ehdr->e_ident[EI_PAD] == 0))
    {
        error(ERROR_NOT_ELF64, woody);
    }
    /*e_ident[EI_DATA] to equal ELFDATA2LSB (little-endian data structures).*/
    if (!(woody->ehdr->e_ident[EI_DATA] == ELFDATA2LSB))
    {
        error(ERROR_ELF_NOT_LITTLE_ENDIAN, woody);
    }
    /*Next check if the EIF type is an executable or a shared library e_type == ET_EXEC or ET_DYN.*/
    if (woody->ehdr->e_type != ET_EXEC && woody->ehdr->e_type != ET_DYN)
    {
        error(ERROR_NOT_EXECUTABLE_BINARY, woody);
    }
}