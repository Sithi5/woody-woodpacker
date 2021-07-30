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
    woody->old_entry_point = woody->ehdr->e_entry;
    // Loop in any program sections to find a PT_NOTE section.
    while (index < woody->ehdr->e_phnum)
    {
        printf("vaddr = %ld\n", woody->phdr[index].p_vaddr);
        if (woody->phdr[index].p_type == PT_NOTE)
        {
            woody->phdr[index].p_type = 0;
            woody->phdr[index].p_flags = PF_R | PF_X;
            woody->phdr[index].p_vaddr = 0xc000000 + (uint64_t)woody->binary_data_len;
            woody->phdr[index].p_filesz = woody->infection_size;
            woody->phdr[index].p_memsz = woody->infection_size;
            woody->phdr[index].p_offset += (uint64_t)woody->binary_data_len;
            printf("previous entry = %ld\n", woody->ehdr->e_entry);
            woody->ehdr->e_entry = woody->phdr[index].p_vaddr;
            printf("new entry = %ld\n", woody->ehdr->e_entry);
        }
        index++;
    }
    printf("end of infection function\n");
}