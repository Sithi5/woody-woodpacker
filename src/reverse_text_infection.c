/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_text_infection.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 14:34:15 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/24 14:34:15 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void reverse_text_infection(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;

    woody->ehdr->e_shoff += PAGE_SIZE;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            woody->phdr[i].p_vaddr -= PAGE_SIZE;
            woody->phdr[i].p_paddr -= PAGE_SIZE;
            woody->phdr[i].p_filesz += PAGE_SIZE;
            woody->phdr[i].p_memsz += PAGE_SIZE;
            woody->ehdr->e_entry = woody->phdr[i].p_vaddr;
        }
        for (int j = i + 1; j < woody->ehdr->e_phnum; j++)
            woody->phdr[j].p_offset += PAGE_SIZE;
    }
    woody->ehdr->e_entry -= PAGE_SIZE;
    woody->ehdr->e_entry += sizeof(t_elf_ehdr);
    woody->ehdr->e_phoff += PAGE_SIZE;

    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        woody->shdr[i].sh_offset += PAGE_SIZE;
    }

    woody->ehdr->e_shoff += PAGE_SIZE;
    woody->ehdr->e_phoff += PAGE_SIZE;

    // Copy until text program end
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->text_p_end_offset);
    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + woody->encrypt_s_start_offset, woody->cipher, woody->encrypt_s_size);
    // Insert payload after text program end
    memcpy(woody->infected_file + woody->text_p_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + woody->text_p_end_offset + PAGE_SIZE, woody->mmap_ptr + woody->text_p_end_offset, woody->binary_data_size - woody->text_p_end_offset);
}