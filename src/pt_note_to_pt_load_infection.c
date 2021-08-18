/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pt_note_to_pt_load_infection.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:13:46 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/18 14:13:46 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void pt_note_to_pt_load_infection(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size;

    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->ehdr->e_ident[EI_PAD + 3] = 7;

    t_elf_off infected_section_start_offset;
    t_elf_off infected_section_end_offset;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_NOTE)
        {
            woody->phdr[i].p_type = PT_LOAD;
            woody->phdr[i].p_flags = (PF_R | PF_X);

            infected_section_start_offset = woody->phdr[i].p_offset;
            infected_section_end_offset = infected_section_start_offset + woody->phdr[i].p_filesz;

            woody->payload_vaddr = woody->phdr[i].p_vaddr;
            woody->ehdr->e_entry = woody->payload_vaddr;
            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;
        }
    }

    for (int i = 0; i < woody->payload_size; i++)
    {
        printf("%x ", ((char *)woody->payload_data)[i]);
    }
    printf("\n");
    printf("\n");
    printf("\n");

    cipher_woody_file_data(woody);
    overwrite_keysection_payload(woody);
    overwrite_payload_ret2textsection(woody);
    overwrite_payload_ret2oep(woody);
    overwrite_payload_settextsectionsize(woody);

    // Insert binary before text section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)infected_section_end_offset);
    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + infected_section_start_offset, woody->cipher, (size_t)(infected_section_end_offset - infected_section_start_offset));
    // Insert payload
    memcpy(woody->infected_file + infected_section_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + infected_section_end_offset + PAGE_SIZE, woody->mmap_ptr + infected_section_end_offset, woody->binary_data_size - infected_section_end_offset);
}