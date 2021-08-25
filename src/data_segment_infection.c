/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_segment_injection.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 11:34:34 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/25 11:34:34 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void data_segment_infection(t_woody *woody)
{
    t_elf_addr data_p_vaddr;
    t_elf_off data_p_start_offset;
    t_elf_off data_p_end_offset;
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++, woody->phdr++)
    {
        /* grab the data segment */
        if (woody->phdr->p_type == PT_LOAD && woody->phdr->p_flags == (PF_R | PF_W))
        {
            woody->phdr->p_flags = (PF_R | PF_W | PF_X);
            data_p_vaddr = woody->phdr->p_vaddr;
            data_p_start_offset = woody->phdr->p_offset;
            data_p_end_offset = data_p_start_offset + woody->phdr->p_filesz;

            woody->payload_vaddr = data_p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = woody->payload_vaddr;
            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr->p_filesz += woody->payload_size;
            woody->phdr->p_memsz += woody->payload_size;
        }
        if (woody->phdr->p_offset > data_p_end_offset)
        {
            woody->phdr->p_offset += PAGE_SIZE;
        }
    }

    /* modify sections here */
    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > data_p_end_offset)
        {
            woody->shdr[i].sh_offset += PAGE_SIZE;
        }
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
        {
            woody->shdr[i].sh_size += woody->payload_size;
        }
    }

    if (woody->ehdr->e_shoff > (data_p_end_offset))
    {
        woody->ehdr->e_shoff += PAGE_SIZE;
    }

    overwrite_payload_ret2oep(woody);

    // Copy until data section end
    ft_memcpy(woody->infected_file, woody->mmap_ptr, data_p_end_offset);
    // Copy parasite
    ft_memcpy(woody->infected_file + data_p_end_offset, woody->payload_data, woody->payload_size);
    // Copy rest of parasite
    ft_memcpy(woody->infected_file + data_p_end_offset + PAGE_SIZE, woody->mmap_ptr + data_p_end_offset, woody->binary_data_size - data_p_end_offset);
}