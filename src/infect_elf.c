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

void set_ptrs(t_woody *woody)
{
    woody->elf64_ptrs->ehdr = (Elf64_Ehdr *)woody->mmap_ptr;
    woody->elf64_ptrs->old_entry_point = woody->elf64_ptrs->ehdr->e_entry;
    woody->elf64_ptrs->phdr = (Elf64_Phdr *)((woody->mmap_ptr + woody->elf64_ptrs->ehdr->e_phoff));
    woody->elf64_ptrs->shdr = (Elf64_Shdr *)((woody->mmap_ptr + woody->elf64_ptrs->ehdr->e_shoff));
}

void infect_elf(t_woody *woody)
{
    if (!(woody->elf64_ptrs = (t_elf64_ptrs *)malloc(sizeof(t_elf64_ptrs))))
        error(ERROR_MALLOC, woody);
    set_ptrs(woody);
    silvio_text_infection(woody);
}