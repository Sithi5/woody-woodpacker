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

void infect_elf(t_woody *woody)
{
    woody->ehdr = (t_elf_ehdr *)woody->mmap_ptr;
    woody->old_entry_point = woody->ehdr->e_entry;
    woody->phdr = (t_elf_phdr *)((woody->mmap_ptr + woody->ehdr->e_phoff));
    woody->shdr = (t_elf_shdr *)((woody->mmap_ptr + woody->ehdr->e_shoff));
    silvio_text_infection(woody);
}