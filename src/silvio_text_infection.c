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
            printf("'\nFOUND LOAD SECTION\n\n");
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
        if (woody->shdr[i].sh_offset > woody->text_end_offset)
            woody->shdr[i].sh_offset += PAGE_SIZE;
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
            woody->shdr[i].sh_size += woody->payload_size;
    }

    // printf("PAYLOAD BEFORE OVERWRITE :\n\n");
    // for (int i = 0; i < woody->payload_size; i++)
    // {
    //     printf("%x ", ((char *)woody->payload_data)[i]);
    // }
    // printf("\n");

    if (ARCH_32)
    {
        printf("\nOVERWRITE ARCH_32\n");
        overwrite_payload_ret2oep(woody);
    }
    else
    {
        overwrite_keysection_payload(woody);
        overwrite_payload_ret2textsection(woody);
        overwrite_payload_ret2oep(woody);
        overwrite_payload_settextsectionsize(woody);
    }
    // printf("\nPAYLOAD AFTER OVERWRITE :\n\n");
    // for (int i = 0; i < woody->payload_size; i++)
    // {
    //     printf("%x ", ((char *)woody->payload_data)[i]);
    // }
    // printf("\n");

    printf("old_entry_point %p\n", woody->old_entry_point);
    printf("new entry point %p\n", woody->new_entry_point);

    // Insert binary before text section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->text_end_offset);

    printf("woody->text_start_offset : %i\n", woody->text_start_offset);

    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + woody->text_start_offset, woody->cipher, (size_t)(woody->text_end_offset - woody->text_start_offset));

    // Insert payload
    memcpy(woody->infected_file + woody->text_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + woody->text_end_offset + PAGE_SIZE, woody->mmap_ptr + woody->text_end_offset, woody->binary_data_size - woody->text_end_offset);
}