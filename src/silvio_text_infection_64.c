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
size_t find_ret2oep_offset_elf64(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
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
                    // Removing 2 to go to actual start of ret2oep (go back to instructions sub).
                    return i - 2;
                }
            }
        }
    }
    error(ERROR_RET2OEP_NOT_FOUND, woody);
    return -1;
}

// Find the ret2textsection offset in the payload. return true if ret2textsection have been found.
size_t find_ret2textsection_offset_elf64(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x66)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2textsection
                if (((char *)woody->payload_data)[i + 1] == 0x66 && ((char *)woody->payload_data)[i + 2] == 0x66 &&
                    ((char *)woody->payload_data)[i + 3] == 0x66 &&
                    ((char *)woody->payload_data)[i + 4] == 0x48 && ((char *)woody->payload_data)[i + 5] == 0x2d &&
                    ((char *)woody->payload_data)[i + 6] == 0x66 && ((char *)woody->payload_data)[i + 7] == 0x66 &&
                    ((char *)woody->payload_data)[i + 8] == 0x66 && ((char *)woody->payload_data)[i + 9] == 0x66 &&
                    ((char *)woody->payload_data)[i + 10] == 0x48 && ((char *)woody->payload_data)[i + 11] == 0x05 &&
                    ((char *)woody->payload_data)[i + 12] == 0x66 && ((char *)woody->payload_data)[i + 13] == 0x66 &&
                    ((char *)woody->payload_data)[i + 14] == 0x66 && ((char *)woody->payload_data)[i + 15] == 0x66)
                {
                    // Removing 2 to go to actual start of ret2textsection (go back to instructions sub).
                    return i - 2;
                }
            }
        }
    }
    error(ERROR_RET2TEXTSECTION_NOT_FOUND, woody);
    return -1;
}

// Find the settextsectionsize offset in the payload. return true if settextsectionsize have been found.
size_t find_settextsectionsize_offset_elf64(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x55)
        {
            if (woody->payload_size - i > 4)
            {
                if (((char *)woody->payload_data)[i + 1] == 0x55 &&
                    ((char *)woody->payload_data)[i + 2] == 0x55 &&
                    ((char *)woody->payload_data)[i + 3] == 0x55)
                {
                    // Removing 2 to go to actual start of settextsectionsize (go back to instructions mov).
                    return i - 2;
                }
            }
        }
    }
    error(ERROR_SETTEXTSECTIONSIZE_NOT_FOUND, woody);
    return -1;
}

// Rewrite info in payload ret2oep.
void overwrite_payload_ret2oep(t_woody *woody)
{
    size_t ret2oep_offset = find_ret2oep_offset_elf64(woody);

    // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
    memcpy(woody->payload_data + ret2oep_offset + 2, (void *)(&(ret2oep_offset)), 4);
    // Rewrite new entry_point in payload ret2oep.
    memcpy(woody->payload_data + ret2oep_offset + 8, (void *)&(woody->elf64_ptrs->new_entry_point), 4);
    // Rewrite old entry_point in payload ret2oep.
    memcpy(woody->payload_data + ret2oep_offset + 14, (void *)&(woody->elf64_ptrs->old_entry_point), 4);
}

// Rewrite info in payload ret2textsection.
void overwrite_payload_ret2textsection(t_woody *woody)
{
    size_t ret2textsection_offset = find_ret2textsection_offset_elf64(woody);
    // Rewrite payload size without ret2textsection. + 2 to skip two first instructions and go to address.
    memcpy(woody->payload_data + ret2textsection_offset + 2, (void *)(&(ret2textsection_offset)), 4);
    // Rewrite new entry_point in payload ret2textsection.
    memcpy(woody->payload_data + ret2textsection_offset + 8, (void *)&(woody->elf64_ptrs->new_entry_point), 4);
    // Rewrite old entry_point in payload ret2textsection.
    memcpy(woody->payload_data + ret2textsection_offset + 14, (void *)&(woody->elf64_ptrs->text_p_vaddr), 4);
}

// Rewrite info in payload settextsectionsize.
void overwrite_payload_settextsectionsize(t_woody *woody)
{
    size_t settextsectionsize_offset = find_settextsectionsize_offset_elf64(woody);
    // Rewrite settextsectionsize_offset + 2 to skip two first instructions and go to textoffset value.
    memcpy(woody->payload_data + settextsectionsize_offset + 2, (void *)&(woody->elf64_ptrs->text_section_size), 4);
}

void silvio_text_infection_elf64(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SZ64)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SZ64;

    load_payload(woody, PAYLOAD_64_NAME);

    if (woody->payload_size > PAGE_SZ64)
    {
        error(ERROR_PAYLOAD_TOO_BIG, woody);
    }

    Elf64_Addr payload_vaddr, text_end_offset;

    // Increase section header offset by PAGE_SIZE
    woody->elf64_ptrs->ehdr->e_shoff += PAGE_SZ64;
    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->elf64_ptrs->ehdr->e_ident[EI_PAD + 3] = 7;

    for (int i = 0; i < woody->elf64_ptrs->ehdr->e_phnum; i++)
    {
        if (woody->elf64_ptrs->phdr[i].p_type == PT_LOAD && woody->elf64_ptrs->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->elf64_ptrs->text_start_offset = woody->elf64_ptrs->phdr[i].p_offset;
            woody->elf64_ptrs->text_end_offset = woody->elf64_ptrs->text_start_offset + woody->elf64_ptrs->phdr[i].p_filesz;
            woody->elf64_ptrs->text_section_size = woody->elf64_ptrs->phdr[i].p_filesz;
            printf("text_section_size = %i\n", woody->elf64_ptrs->text_section_size);

            woody->elf64_ptrs->text_p_vaddr = woody->elf64_ptrs->phdr[i].p_vaddr;
            woody->elf64_ptrs->payload_vaddr = woody->elf64_ptrs->text_p_vaddr + woody->elf64_ptrs->phdr[i].p_filesz;
            woody->elf64_ptrs->ehdr->e_entry = woody->elf64_ptrs->payload_vaddr;
            woody->elf64_ptrs->new_entry_point = woody->elf64_ptrs->payload_vaddr;

            woody->elf64_ptrs->phdr[i].p_filesz += woody->payload_size;
            woody->elf64_ptrs->phdr[i].p_memsz += woody->payload_size;

            for (int j = i + 1; j < woody->elf64_ptrs->ehdr->e_phnum; j++)
                woody->elf64_ptrs->phdr[j].p_offset += PAGE_SZ64;

            break;
        }
    }

    // Adding offset of one page in all section located after text section end.
    for (int i = 0; i < woody->elf64_ptrs->ehdr->e_shnum; i++)
    {
        if (woody->elf64_ptrs->shdr[i].sh_offset > woody->elf64_ptrs->text_section_size)
            woody->elf64_ptrs->shdr[i].sh_offset += PAGE_SZ64;
        else if (woody->elf64_ptrs->shdr[i].sh_addr + woody->elf64_ptrs->shdr[i].sh_size == woody->elf64_ptrs->payload_vaddr)
            woody->elf64_ptrs->shdr[i].sh_size += woody->payload_size;
    }

    cipher_woody_file_data(woody);
    overwrite_payload_ret2textsection(woody);
    overwrite_payload_ret2oep(woody);
    overwrite_payload_settextsectionsize(woody);

    // Insert binary before text section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->elf64_ptrs->text_end_offset);
    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + woody->elf64_ptrs->text_start_offset, woody->cipher, (size_t)(woody->elf64_ptrs->text_end_offset - woody->elf64_ptrs->text_start_offset));
    // Insert payload
    memcpy(woody->infected_file + woody->elf64_ptrs->text_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + woody->elf64_ptrs->text_end_offset + PAGE_SZ64, woody->mmap_ptr + woody->elf64_ptrs->text_end_offset, woody->binary_data_size - woody->elf64_ptrs->text_end_offset);
}