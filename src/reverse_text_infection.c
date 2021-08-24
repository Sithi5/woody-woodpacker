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

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            printf("woody->phdr[i].p_vaddr %p\n", woody->phdr[i].p_vaddr);
            printf("woody->phdr[i].p_paddr %p\n", woody->phdr[i].p_paddr);
            // woody->phdr[i].p_vaddr -= PAGE_SIZE;
            // woody->phdr[i].p_paddr -= PAGE_SIZE;
            // woody->phdr[i].p_filesz += PAGE_SIZE;
            // woody->phdr[i].p_memsz += PAGE_SIZE;
            // woody->ehdr->e_entry = woody->phdr[i].p_vaddr;
        }
        // woody->phdr[i].p_offset += PAGE_SIZE;
    }

    // for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    // {
    //     if (!strncmp(TEXT_SECTION_NAME,
    //                  (woody->string_table_ptr + woody->shdr[i].sh_name),
    //                  strlen(TEXT_SECTION_NAME)))
    //     {
    //         woody->shdr[i].sh_addr = woody->ehdr->e_entry - PAGE_SIZE;
    //         woody->shdr[i].sh_addr += PAGE_SIZE;
    //         woody->shdr[i].sh_size += woody->payload_size;
    //     }
    //     woody->shdr[i].sh_offset += PAGE_SIZE;
    // }

    // woody->ehdr->e_entry -= PAGE_SIZE;
    // woody->ehdr->e_entry += sizeof(t_elf_ehdr);
    // woody->ehdr->e_phoff += PAGE_SIZE;
    // woody->ehdr->e_shoff += PAGE_SIZE;

    // Insert header
    memcpy(woody->infected_file, woody->mmap_ptr, woody->binary_data_size);
    // // Insert payload
    // memcpy(woody->infected_file + sizeof(t_elf_ehdr), woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + sizeof(t_elf_ehdr), woody->mmap_ptr + sizeof(t_elf_ehdr), woody->binary_data_size - sizeof(t_elf_ehdr));
}