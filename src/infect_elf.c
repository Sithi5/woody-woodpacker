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
    woody->ehdr = (Elf64_Ehdr *)woody->mmap_ptr;
    woody->old_entry_point = woody->ehdr->e_entry;
    woody->phdr = (Elf64_Phdr *)((woody->mmap_ptr + woody->ehdr->e_phoff));
    woody->shdr = (Elf64_Shdr *)((woody->mmap_ptr + woody->ehdr->e_shoff));
}

void infect_elf(t_woody *woody)
{
    set_ptrs(woody);
    silvio_text_infection(woody);
}