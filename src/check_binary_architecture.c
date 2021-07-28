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
    if (!((header->e_type == ET_EXEC || header->e_type == ET_DYN) && header->e_ident[1] == ELFMAG1 &&
          header->e_ident[2] == ELFMAG2 &&
          header->e_ident[3] == ELFMAG3 &&          /*Identify executable and ELF*/
          header->e_ident[EI_CLASS] == ELFCLASS64)) /*architecture 64 bits*/
    {
        error(ERROR_NOT_ELF64, woody);
    }
    woody->addr_entrypoint = (void *)header->e_entry;
}
