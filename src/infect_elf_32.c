/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect_elf_32.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 15:08:25 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/09 15:08:25 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void set_elf32_ptrs(t_woody *woody)
{
    woody->elf32_ptrs->ehdr = (Elf32_Ehdr *)woody->mmap_ptr;
    woody->elf32_ptrs->old_entry_point = woody->elf32_ptrs->ehdr->e_entry;
    woody->elf32_ptrs->phdr = (Elf32_Phdr *)((woody->mmap_ptr + woody->elf32_ptrs->ehdr->e_phoff));
    woody->elf32_ptrs->shdr = (Elf32_Shdr *)((woody->mmap_ptr + woody->elf32_ptrs->ehdr->e_shoff));
}

void infect_elf_32(t_woody *woody)
{
    if (!(woody->elf32_ptrs = (t_elf32_ptrs *)malloc(sizeof(t_elf32_ptrs))))
        error(ERROR_MALLOC, woody);
    set_elf32_ptrs(woody);
    silvio_text_infection_elf32(woody);
}