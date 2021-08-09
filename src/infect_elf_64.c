/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect_elf_64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 15:04:58 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/09 15:04:58 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void set_elf64_ptrs(t_woody *woody)
{
    woody->elf64_ptrs->ehdr = (Elf64_Ehdr *)woody->mmap_ptr;
    woody->elf64_ptrs->old_entry_point = woody->elf64_ptrs->ehdr->e_entry;
    woody->elf64_ptrs->phdr = (Elf64_Phdr *)((woody->mmap_ptr + woody->elf64_ptrs->ehdr->e_phoff));
    woody->elf64_ptrs->shdr = (Elf64_Shdr *)((woody->mmap_ptr + woody->elf64_ptrs->ehdr->e_shoff));
}

void check_ehdr_elf64(t_woody *woody)
{
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

void infect_elf_64(t_woody *woody)
{
    if (!(woody->elf64_ptrs = (t_elf64_ptrs *)malloc(sizeof(t_elf64_ptrs))))
        error(ERROR_MALLOC, woody);
    set_elf64_ptrs(woody);
}