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
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;

    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SIZE;
    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->ehdr->e_ident[EI_PAD + 3] = 7;

    t_elf_off infected_section_start_offset;
    t_elf_off infected_section_p_vaddr;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_NOTE)
        {
            woody->phdr[i].p_type = PT_LOAD;
            woody->phdr[i].p_flags = (PF_R | PF_X);

            infected_section_start_offset = woody->phdr[i].p_offset;
            infected_section_p_vaddr = woody->phdr[i].p_vaddr;

            woody->payload_vaddr = infected_section_p_vaddr;
            woody->ehdr->e_entry = woody->payload_vaddr;
            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr[i].p_memsz = woody->payload_size;

            for (int j = i + 1; j < woody->ehdr->e_phnum; j++)
                woody->phdr[j].p_offset += PAGE_SIZE;

            break;
        }
    }

    // Adding offset of one page in all section located after text section end.
    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > woody->text_end_offset)
            woody->shdr[i].sh_offset += PAGE_SIZE;
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
            woody->shdr[i].sh_size += woody->payload_size;
    }

    printf("LAAA\n");
    overwrite_keysection_payload(woody);
    overwrite_payload_ret2textsection(woody);
    overwrite_payload_ret2oep(woody);
    overwrite_payload_settextsectionsize(woody);

    printf("ICI\n");

    // Insert binary before text section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->text_start_offset);

    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + woody->text_start_offset, woody->cipher, (size_t)(woody->text_end_offset - woody->text_start_offset));

    // Copy until reach section PT_NOTE
    memcpy(woody->infected_file + woody->text_end_offset, woody->mmap_ptr + woody->text_end_offset, (size_t)(infected_section_start_offset - woody->text_end_offset));

    // Insert payload
    memcpy(woody->infected_file + infected_section_start_offset, woody->payload_data, woody->payload_size);

    // Insert rest of binary
    memcpy(woody->infected_file + infected_section_start_offset + PAGE_SIZE, woody->mmap_ptr + infected_section_start_offset + woody->payload_size, woody->binary_data_size - (infected_section_start_offset + woody->payload_size));
}