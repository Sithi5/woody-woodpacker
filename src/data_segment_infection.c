/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_segment_injection.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 11:34:34 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/25 11:34:34 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void data_segment_infection(t_woody *woody)
{
    printf("INSIDE DATASEGMENT\n");

    t_elf_addr data_p_vaddr;
    t_elf_off data_p_start_offset;
    t_elf_off data_p_end_offset;
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;
    woody->encrypt_s_size = 0;

    data_p_end_offset = 0;
    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        /* grab the data segment */
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_W))
        {
            woody->phdr[i].p_flags = (PF_R | PF_W | PF_X);
            data_p_vaddr = woody->phdr[i].p_vaddr;
            data_p_start_offset = woody->phdr[i].p_offset;
            data_p_end_offset = data_p_start_offset + woody->phdr[i].p_filesz;

            woody->payload_vaddr = data_p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = woody->payload_vaddr;
            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;
        }
        if (woody->phdr[i].p_offset > data_p_end_offset)
        {
            woody->phdr[i].p_offset += PAGE_SIZE;
        }
    }

    if (data_p_end_offset % PAGE_SIZE + woody->payload_size > PAGE_SIZE)
    {
        error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, woody);
    }

    /* modify sections here */
    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > data_p_end_offset)
        {
            woody->shdr[i].sh_offset += PAGE_SIZE;
        }
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
        {
            woody->shdr[i].sh_size += woody->payload_size;
        }
        // get section to encrypt info.
        if (!ft_strncmp(SECTION_TO_ENCRYPT_NAME,
                        (woody->string_table_ptr + woody->shdr[i].sh_name),
                        strlen(SECTION_TO_ENCRYPT_NAME)))
        {
            woody->encrypt_s_start_offset = woody->shdr[i].sh_offset;
            woody->encrypt_s_size = woody->shdr[i].sh_size;
            woody->encrypt_s_end_offset = woody->encrypt_s_start_offset + woody->encrypt_s_size;
            woody->encrypt_s_addr = woody->shdr[i].sh_addr;
            woody->shdr[i].sh_flags |= SHF_WRITE;
        }
    }

    if (woody->ehdr->e_shoff > (data_p_end_offset))
    {
        woody->ehdr->e_shoff += PAGE_SIZE;
    }

    cipher_woody_file_data(woody);

    if (ARCH_32)
    {
        overwrite_payload_ret2oep(woody);
    }
    else if (ARCH_64)
    {
        overwrite_keysection_payload(woody);
        overwrite_payload_getencryptedsectionaddr(woody);
        overwrite_payload_ret2oep(woody);
        overwrite_payload_getencryptedsectionsize(woody);
        overwrite_payload_gettextsectionaddr(woody);
        overwrite_payload_gettextsize(woody);
    }

    // Copy until text section end
    ft_memcpy(woody->infected_file,
              woody->mmap_ptr,
              woody->text_p_end_offset);
    // Rewrite text section with cipher data.
    ft_memcpy(woody->infected_file + woody->encrypt_s_start_offset,
              woody->cipher,
              woody->encrypt_s_size);
    // Copy until data section end
    ft_memcpy(woody->infected_file + woody->text_p_end_offset,
              woody->mmap_ptr + woody->text_p_end_offset,
              data_p_end_offset - woody->text_p_end_offset);
    // Copy parasite
    ft_memcpy(woody->infected_file + data_p_end_offset,
              woody->payload_data,
              woody->payload_size);
    // Copy rest of file
    ft_memcpy(woody->infected_file + data_p_end_offset + PAGE_SIZE,
              woody->mmap_ptr + data_p_end_offset,
              woody->binary_data_size - data_p_end_offset);
}