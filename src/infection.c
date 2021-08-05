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

void load_payload(t_woody *woody, char *payload_name)
{
    (void)payload_name;
    // char *code = "\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x0a\x00\x0d\x00\x00\x50\x51\x52\x56\x57\x41\x53\x48\x31\xd2\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\xba\x0d\x00\x00\x00\x48\x8d\x35\xcf\xff\xff\xff\x0f\x05";
    // woody->payload_size = 50;
    char *code = "";
    woody->payload_size = 0;

    if (!(woody->payload_data = (char *)malloc(sizeof(char) * woody->payload_size)))
        error(ERROR_MALLOC, woody);
    memcpy(woody->payload_data, code, woody->payload_size);
    printf("debug\n");
}

void silvio_text_infection(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SZ64)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SZ64;

    Elf64_Addr payload_vaddr, text_end_offset;
    char jump_entry[] = "\x68\x50\x10\x40\x00\xc3"; // push ret
    // char jump_entry[] = "\x48\xb8\x41\x41\x41\x41\x41\x41\x41\x41" //mov rax,0x4141414141414141
    //                     "\xff\xe0";                                // jmp rax
    int jump_size = 6;
    woody->payload_size = 0;
    woody->payload_size += jump_size;

    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SZ64;
    printf("old entry: %p\n", woody->ehdr->e_entry);

    for (int i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            text_end_offset = woody->phdr[i].p_offset + woody->phdr[i].p_filesz;

            payload_vaddr = woody->phdr[i].p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = payload_vaddr;
            printf("new entry: %p\n", woody->ehdr->e_entry);
            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;

            for (int j = i + 1; j < woody->ehdr->e_phnum; j++)
                woody->phdr[j].p_offset += PAGE_SZ64;

            break;
        }
    }

    // Adding offset of one page in all section located after text section end.
    for (int i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > text_end_offset)
            woody->shdr[i].sh_offset += PAGE_SZ64;
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == payload_vaddr)
            woody->shdr[i].sh_size += woody->payload_size;
    }

    // load the payload to insert after text.

    load_payload(woody, PAYLOAD_NAME);
    if (woody->payload_size > PAGE_SZ64)
    {
        error(ERROR_NOT_DEFINED, woody);
    }

    // Patch the jump with the old_entry_point vaddr
    // memcpy(&jump_entry[1], (char *)&woody->old_entry_point, 4);
    printf("debug2\n");

    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)text_end_offset);
    printf("debug3\n");

    // memcpy(woody->infected_file + text_end_offset, woody->payload_data, woody->payload_size - jump_size);
    printf("debug4\n");

    memcpy(woody->infected_file + text_end_offset + woody->payload_size - jump_size, jump_entry, jump_size);

    memcpy(woody->infected_file + text_end_offset + PAGE_SZ64, woody->mmap_ptr + text_end_offset, woody->binary_data_size - text_end_offset);
}