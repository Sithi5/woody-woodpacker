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
bool find_ret2oep_offset(t_woody *woody)
{
    for (uint32_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x77)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2oep
                if (((char *)woody->payload_data)[i + 1] == 0x77 && ((char *)woody->payload_data)[i + 2] == 0x77 &&
                    ((char *)woody->payload_data)[i + 3] == 0x77 &&
                    ((char *)woody->payload_data)[i + 4] == 0x48 && ((char *)woody->payload_data)[i + 5] == 0x2d &&
                    ((char *)woody->payload_data)[i + 6] == 0x77 && ((char *)woody->payload_data)[i + 7] == 0x77 &&
                    ((char *)woody->payload_data)[i + 8] == 0x77 && ((char *)woody->payload_data)[i + 9] == 0x77 &&
                    ((char *)woody->payload_data)[i + 10] == 0x48 && ((char *)woody->payload_data)[i + 11] == 0x05 &&
                    ((char *)woody->payload_data)[i + 12] == 0x77 && ((char *)woody->payload_data)[i + 13] == 0x77 &&
                    ((char *)woody->payload_data)[i + 14] == 0x77 && ((char *)woody->payload_data)[i + 15] == 0x77)
                {
                    // Removing 3 to go to actual start of ret2oep.
                    woody->ret2oep_offset = i - 2;
                    return true;
                }
            }
        }
    }
    return false;
}

void silvio_text_infection_elf32(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SZ64)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SZ64;

    load_payload(woody, PAYLOAD_NAME);

    if (woody->payload_size > PAGE_SZ64)
    {
        error(ERROR_NOT_DEFINED, woody);
    }

    Elf64_Addr payload_vaddr, text_end_offset;

    // Increase section header offset by PAGE_SIZE
    woody->elf32_ptrs->ehdr->e_shoff += PAGE_SZ64;
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
                woody->elf32_ptrs->phdr[j].p_offset += PAGE_SZ64;

            break;
        }
    }

    // Adding offset of one page in all section located after text section end.
    for (int i = 0; i < woody->elf32_ptrs->ehdr->e_shnum; i++)
    {
        if (woody->elf32_ptrs->shdr[i].sh_offset > text_end_offset)
            woody->elf32_ptrs->shdr[i].sh_offset += PAGE_SZ64;
        else if (woody->elf32_ptrs->shdr[i].sh_addr + woody->elf32_ptrs->shdr[i].sh_size == payload_vaddr)
            woody->elf32_ptrs->shdr[i].sh_size += woody->payload_size;
    }

    if (!find_ret2oep_offset(woody))
    {
        error(ERROR_RET2OEP_NOT_FOUND, woody);
    }

    // Rewrite info in payload ret2oep.
    // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
    memcpy(woody->payload_data + woody->ret2oep_offset + 2, (void *)(&(woody->ret2oep_offset)), 4);
    // Rewrite new entry_point in payload ret2oep.
    memcpy(woody->payload_data + woody->ret2oep_offset + 8, (void *)&(woody->elf32_ptrs->new_entry_point), 4);
    // Rewrite old entry_point in payload ret2oep.
    memcpy(woody->payload_data + woody->ret2oep_offset + 14, (void *)&(woody->elf32_ptrs->old_entry_point), 4);

    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)text_end_offset);
    memcpy(woody->infected_file + text_end_offset, woody->payload_data, woody->payload_size);
    memcpy(woody->infected_file + text_end_offset + PAGE_SZ64, woody->mmap_ptr + text_end_offset, woody->binary_data_size - text_end_offset);
}