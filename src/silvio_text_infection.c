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

void silvio_text_infection(t_woody *woody)
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

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            woody->text_p_vaddr = woody->phdr[i].p_vaddr;
            woody->payload_vaddr = woody->text_p_vaddr + woody->phdr[i].p_filesz;

            woody->ehdr->e_entry = woody->payload_vaddr;

            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;

            for (int j = i + 1; j < woody->ehdr->e_phnum; j++)
                woody->phdr[j].p_offset += PAGE_SIZE;

            break;
        }
    }

    // Adding offset of one page in all section located after text section end.
    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > woody->text_p_end_offset)
        {
            woody->shdr[i].sh_offset += PAGE_SIZE;
        }
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
        {
            woody->text_s_offset = woody->shdr[i].sh_offset;
            woody->text_s_size = woody->shdr[i].sh_size;
            woody->shdr[i].sh_size += woody->payload_size;
        }
    }

    if (ARCH_32)
    {
        overwrite_payload_ret2oep(woody);
    }
    else if (ARCH_64)
    {
        overwrite_keysection_payload(woody);
        overwrite_payload_ret2textsection(woody);
        overwrite_payload_ret2oep(woody);
        overwrite_payload_settextsectionsize(woody);
    }

    printf("\n");
    printf("Offsets:\n");
    printf("\twoody->text_s_offset:\t\t\t\t\t%lu\n", woody->text_s_offset);
    printf("\twoody->text_s_size:\t\t\t\t\t%lu\n", woody->text_s_size);
    printf("Adresses:\n");
    printf("\twoody->old_entry_point:\t\t\t\t0x%lx -> %lu\n", woody->old_entry_point, woody->old_entry_point);
    printf("\twoody->new_entry_point:\t\t\t\t0x%lx -> %lu\n", woody->new_entry_point, woody->new_entry_point);
    printf("\twoody->text_p_vaddr:\t\t\t\t0x%lx -> %lu\n", woody->text_p_vaddr, woody->text_p_vaddr);
    printf("\n");

    // Copy until text end section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->text_p_end_offset);

    if (ARCH_64)
    {
        // Rewrite text section with cipher data.
        memcpy(woody->infected_file + woody->text_p_start_offset, woody->cipher, woody->cipher_size);
    }
    // Insert payload after text end section
    memcpy(woody->infected_file + woody->text_p_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + woody->text_p_end_offset + PAGE_SIZE, woody->mmap_ptr + woody->text_p_end_offset, woody->binary_data_size - woody->text_p_end_offset);
}