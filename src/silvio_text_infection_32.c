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

// Find the ret2oep offset in the payload. return true if ret2oep have been found.
size_t find_ret2oep_offset_elf32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x77)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2oep
                if (((char *)woody->payload_data)[i + 1] == 0x77 && ((char *)woody->payload_data)[i + 2] == 0x77 &&
                    ((char *)woody->payload_data)[i + 3] == 0x77 && ((char *)woody->payload_data)[i + 4] == 0x2d &&
                    ((char *)woody->payload_data)[i + 5] == 0x77 && ((char *)woody->payload_data)[i + 6] == 0x77 &&
                    ((char *)woody->payload_data)[i + 7] == 0x77 && ((char *)woody->payload_data)[i + 8] == 0x77 &&
                    ((char *)woody->payload_data)[i + 9] == 0x05 &&
                    ((char *)woody->payload_data)[i + 10] == 0x77 && ((char *)woody->payload_data)[i + 11] == 0x77 &&
                    ((char *)woody->payload_data)[i + 12] == 0x77 && ((char *)woody->payload_data)[i + 13] == 0x77)
                {
                    // Removing 1 to go to actual start of ret2oep (go back to instructions sub).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_RET2OEP_NOT_FOUND, woody);
    return -1;
}

void silvio_text_infection_elf32(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SZ32)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SZ32;

    load_payload(woody, PAYLOAD_32_NAME);

    if (woody->payload_size > PAGE_SZ32)
    {
        error(ERROR_PAYLOAD_TOO_BIG, woody);
    }

    Elf64_Addr payload_vaddr, text_end_offset;

    for (uint32_t i = 0; i < woody->payload_size; i++)
    {
        printf("%x ", ((char *)(woody->payload_data))[i]);
    }

    // Increase section header offset by PAGE_SIZE
    woody->elf32_ptrs->ehdr->e_shoff += PAGE_SZ32;
    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->elf32_ptrs->ehdr->e_ident[EI_PAD + 3] = 7;

    for (int i = 0; i < woody->elf32_ptrs->ehdr->e_phnum; i++)
    {
        if (woody->elf32_ptrs->phdr[i].p_type == PT_LOAD && woody->elf32_ptrs->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            text_end_offset = woody->elf32_ptrs->phdr[i].p_offset + woody->elf32_ptrs->phdr[i].p_filesz;
            payload_vaddr = woody->elf32_ptrs->phdr[i].p_vaddr + woody->elf32_ptrs->phdr[i].p_filesz;
            woody->elf32_ptrs->ehdr->e_entry = payload_vaddr;
            woody->elf32_ptrs->new_entry_point = payload_vaddr;
            woody->elf32_ptrs->phdr[i].p_filesz += woody->payload_size;
            woody->elf32_ptrs->phdr[i].p_memsz += woody->payload_size;

            for (int j = i + 1; j < woody->elf32_ptrs->ehdr->e_phnum; j++)
                woody->elf32_ptrs->phdr[j].p_offset += PAGE_SZ32;

            break;
        }
    }

    // Adding offset of one page in all section located after text section end.
    for (int i = 0; i < woody->elf32_ptrs->ehdr->e_shnum; i++)
    {
        if (woody->elf32_ptrs->shdr[i].sh_offset > text_end_offset)
            woody->elf32_ptrs->shdr[i].sh_offset += PAGE_SZ32;
        else if (woody->elf32_ptrs->shdr[i].sh_addr + woody->elf32_ptrs->shdr[i].sh_size == payload_vaddr)
            woody->elf32_ptrs->shdr[i].sh_size += woody->payload_size;
    }

    size_t ret2oep_offset = find_ret2oep_offset_elf32(woody);
    printf("\n\nret2oep_offset = %i\n", ret2oep_offset);

    //

    for (uint32_t i = 0; i < woody->payload_size; i++)
    {
        printf("%x ", ((char *)(woody->payload_data))[i]);
    }
    printf("\n\nEntry virus size: ");
    for (uint32_t i = 0; i < 4; i++)
    {
        printf("%x ", ((char *)(woody->payload_data))[ret2oep_offset + 3 + i]);
    }
    printf("\n\nEntry NEW in VX: ");
    for (uint32_t i = 0; i < 4; i++)
    {
        printf("%x ", ((char *)(woody->payload_data))[ret2oep_offset + 9 + i]);
    }

    printf("\n");
    printf("\nEntry OLD in VX: ");
    for (uint32_t i = 0; i < 4; i++)
    {
        printf("%x ", ((char *)(woody->payload_data))[ret2oep_offset + 15 + i]);
    }
    printf("\n\n");

    //
    // Rewrite info in payload ret2oep.
    // Rewrite payload size without ret2oep. + 1 to skip first instructions and go to address.
    memcpy(woody->payload_data + ret2oep_offset + 1, (void *)(&(ret2oep_offset)), 4);
    // Rewrite new entry_point in payload ret2oep.
    memcpy(woody->payload_data + ret2oep_offset + 6, (void *)&(woody->elf32_ptrs->new_entry_point), 4);
    // Rewrite old entry_point in payload ret2oep.
    memcpy(woody->payload_data + ret2oep_offset + 11, (void *)&(woody->elf32_ptrs->old_entry_point), 4);

    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)text_end_offset);
    memcpy(woody->infected_file + text_end_offset, woody->payload_data, woody->payload_size);
    memcpy(woody->infected_file + text_end_offset + PAGE_SZ32, woody->mmap_ptr + text_end_offset, woody->binary_data_size - text_end_offset);
}