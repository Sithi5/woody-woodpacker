/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 14:29:35 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/29 14:29:35 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void set_infection_size(t_woody *woody)
{
    woody->infection_size = 10;
}

void elf64_pt_note_to_pt_load_infection(t_woody *woody)
{
    printf("start of infection function\n");
    int index = 0;
    set_infection_size(woody);
    woody->old_entry_point = woody->elf_header->e_entry;
    // Loop in any program sections to find a PT_NOTE section.
    while (index < woody->elf_header->e_phnum)
    {
        printf("vaddr = %ld\n", woody->program_header_table[index].p_vaddr);
        if (woody->program_header_table[index].p_type == PT_NOTE)
        {
            woody->program_header_table[index].p_type = 0;
            woody->program_header_table[index].p_flags = PF_R | PF_X;
            woody->program_header_table[index].p_vaddr = 0xc000000 + (uint64_t)woody->binary_data_len;
            woody->program_header_table[index].p_filesz += woody->infection_size;
            woody->program_header_table[index].p_memsz += woody->infection_size;
            woody->program_header_table[index].p_offset += (uint64_t)woody->binary_data_len;
            // Set the new entrypoint to be this program sections
            printf("previous entry = %ld\n", woody->elf_header->e_entry);
            woody->elf_header->e_entry = woody->program_header_table[index].p_vaddr;
            printf("new entry = %ld\n", woody->elf_header->e_entry);
        }
        index++;
    }
    printf("end of infection function\n");
}