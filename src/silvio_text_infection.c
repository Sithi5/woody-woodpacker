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

size_t find_keysection_offset(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x44)
        {
            if (woody->payload_size - i > 128)
            {
                if (((char *)woody->payload_data)[i + 1] == 0x44 && ((char *)woody->payload_data)[i + 2] == 0x44 && ((char *)woody->payload_data)[i + 3] == 0x44)
                {
                    return i;
                }
            }
        }
    }
    error(ERROR_KEYSECTION_NOT_FOUND, woody);
    return -1;
}

// Find the ret2oep offset in the payload. return true if ret2oep have been found.
size_t find_ret2oep_offset(t_woody *woody)
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
    size_t ret2oep_offset = find_ret2oep_offset(woody);

    // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
    memcpy(woody->payload_data + ret2oep_offset + 2, (void *)(&(ret2oep_offset)), 4);
    // Rewrite new entry_point in payload ret2oep.
    memcpy(woody->payload_data + ret2oep_offset + 8, (void *)&(woody->new_entry_point), 4);
    // Rewrite old entry_point in payload ret2oep.
    memcpy(woody->payload_data + ret2oep_offset + 14, (void *)&(woody->old_entry_point), 4);
}

// Rewrite info in payload ret2textsection.
void overwrite_payload_ret2textsection(t_woody *woody)
{
    size_t ret2textsection_offset = find_ret2textsection_offset_elf64(woody);
    // Rewrite payload size without ret2textsection. + 2 to skip two first instructions and go to address.
    memcpy(woody->payload_data + ret2textsection_offset + 2, (void *)(&(ret2textsection_offset)), 4);
    // Rewrite new entry_point in payload ret2textsection.
    memcpy(woody->payload_data + ret2textsection_offset + 8, (void *)&(woody->new_entry_point), 4);
    // Rewrite old entry_point in payload ret2textsection.
    memcpy(woody->payload_data + ret2textsection_offset + 14, (void *)&(woody->text_p_vaddr), 4);
}

// Rewrite info in payload settextsectionsize.
void overwrite_payload_settextsectionsize(t_woody *woody)
{
    size_t settextsectionsize_offset = find_settextsectionsize_offset_elf64(woody);
    // Rewrite settextsectionsize_offset + 2 to skip two first instructions and go to textoffset value.
    memcpy(woody->payload_data + settextsectionsize_offset + 2, (void *)&(woody->text_section_size), 4);
}

void overwrite_keysection_payload(t_woody *woody)
{
    size_t keysection_offset = find_keysection_offset(woody);
    printf("\n\n");
    memcpy(woody->payload_data + keysection_offset, woody->encryption_key, KEY_LEN);
}

void choose_infection_method(t_woody *woody)
{
    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->text_start_offset = woody->phdr[i].p_offset;
            woody->text_end_offset = woody->text_start_offset + woody->phdr[i].p_filesz;
            woody->text_section_size = woody->phdr[i].p_filesz;

            // Check if there is enought space for our payload in the text section.
            if (woody->text_end_offset % PAGE_SIZE + woody->payload_size < PAGE_SIZE)
            {
                silvio_text_infection(woody);
            }
            else
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, woody);
            }
        }
    }
}

void silvio_text_infection(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;

    load_payload(woody, PAYLOAD_NAME);

    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SIZE;
    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->ehdr->e_ident[EI_PAD + 3] = 7;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->text_start_offset = woody->phdr[i].p_offset;
            woody->text_end_offset = woody->text_start_offset + woody->phdr[i].p_filesz;
            woody->text_section_size = woody->phdr[i].p_filesz;

            // Check if there is enought space for our payload.
            if (woody->text_end_offset % PAGE_SIZE + woody->payload_size > PAGE_SIZE)
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, woody);
            }

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

    cipher_woody_file_data(woody);
    overwrite_keysection_payload(woody);
    overwrite_payload_ret2textsection(woody);
    overwrite_payload_ret2oep(woody);
    overwrite_payload_settextsectionsize(woody);

    // Insert binary before text section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->text_end_offset);
    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + woody->text_start_offset, woody->cipher, (size_t)(woody->text_end_offset - woody->text_start_offset));
    // Insert payload
    memcpy(woody->infected_file + woody->text_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + woody->text_end_offset + PAGE_SIZE, woody->mmap_ptr + woody->text_end_offset, woody->binary_data_size - woody->text_end_offset);
}