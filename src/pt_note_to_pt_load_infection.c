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
    printf("PT_NOTE_TO_PT_LOAD_INFECTION\n\n");
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;

    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->ehdr->e_ident[EI_PAD + 3] = 7;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_NOTE)
        {
            woody->phdr[i].p_type = PT_LOAD;
            woody->phdr[i].p_flags = (PF_R | PF_X);

            woody->phdr[i].p_vaddr = 0xc000000 + woody->binary_data_size;
            woody->phdr[i].p_memsz = woody->payload_size;
            woody->phdr[i].p_filesz = woody->payload_size;
            woody->phdr[i].p_offset = woody->binary_data_size;

            woody->ehdr->e_entry = woody->phdr[i].p_vaddr;

            break;
        }
    }
    overwrite_payload_ret2oep(woody);

    cipher_woody_file_data(woody);

    // Copy entirely the original binary file
    memcpy(woody->infected_file, woody->mmap_ptr, woody->binary_data_size);
    // Copy payload at end of binary file
    // memcpy(woody->infected_file + woody->binary_data_size, woody->payload_data, woody->payload_size);
}