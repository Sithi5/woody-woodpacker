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

void find_text_and_set_offset(t_woody *woody)
{
    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->text_start_offset = woody->phdr[i].p_offset;
            woody->text_end_offset = woody->text_start_offset + woody->phdr[i].p_filesz;
            woody->text_section_size = woody->phdr[i].p_filesz;

            printf("PAGE_SIZE = %i\n", PAGE_SIZE);
            printf("woody->text_end_offset = %li\n", woody->text_end_offset);
            printf("woody->text_end_offset modulo PAGE_SIZE = %li\n", woody->text_end_offset % PAGE_SIZE);
            printf("woody->text_end_offset modulo PAGE_SIZE + payload_size = %li\n", woody->text_end_offset % PAGE_SIZE + woody->payload_size);

            woody->text_p_vaddr = woody->phdr[i].p_vaddr;
            woody->payload_vaddr = woody->text_p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = woody->payload_vaddr;
            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;

            for (size_t j = i + 1; j < woody->ehdr->e_phnum; j++)
                woody->phdr[j].p_offset += PAGE_SIZE;

            break;
        }
    }
}

void fix_section_offset(t_woody *woody)
{
    // Adding offset of one page in all section located after text section end.
    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > woody->text_end_offset)
            woody->shdr[i].sh_offset += PAGE_SIZE;
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
            woody->shdr[i].sh_size += woody->payload_size;
    }
}



void silvio_text_infection(t_woody *woody)
{

    get_payload(woody);
    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SIZE;
    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->ehdr->e_ident[EI_PAD + 3] = 7;

    find_text_and_set_offset(woody);
    fix_section_offset(woody);
    cipher_woody_file_data(woody);
    overwrite_payload_ret2textsection(woody);
    overwrite_payload_ret2oep(woody);
    overwrite_payload_settextsectionsize(woody);
    assemble_infected_file(woody);
}