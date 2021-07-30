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

// TODO: Not sure of this value, should create a function to get page size.
#define PAGE_SZ64 0x2000

void set_payload_size(t_woody *woody)
{
    // Do this function or set payload size elsewhere.
    woody->payload_size = 10;
}

void elf64_pt_note_to_pt_load_infection(t_woody *woody)
{
    printf("start of infection function\n");
    int index = 0;
    set_payload_size(woody);
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
            woody->phdr[index].p_filesz = woody->payload_size;
            woody->phdr[index].p_memsz = woody->payload_size;
            woody->phdr[index].p_offset += (uint64_t)woody->binary_data_len;
            printf("previous entry = %ld\n", woody->ehdr->e_entry);
            woody->ehdr->e_entry = woody->phdr[index].p_vaddr;
            printf("new entry = %ld\n", woody->ehdr->e_entry);
        }
        index++;
    }
    printf("end of infection function\n");
}

void silvio_text_infection(t_woody *woody)
{
    Elf64_Addr payload_vaddr, text_end;
    char jmp_entry[] = "\x48\xb8\x41\x41\x41\x41\x41\x41\x41\x41" //mov rax,0x4141414141414141
                       "\xff\xe0";                                // jmp rax
    int jmp_len = 12;

    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SZ64;

    for (int i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here;
            text_end = woody->phdr[i].p_offset + woody->phdr[i].p_filesz;

            payload_vaddr = woody->phdr[i].p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = payload_vaddr;
            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;

            for (int j = i + 1; j < woody->ehdr->e_phnum; j++)
                woody->phdr[j].p_offset += PAGE_SZ64;

            break;
        }
    }

    for (int i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > text_end)
            woody->shdr[i].sh_offset += PAGE_SZ64;

        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == payload_vaddr)
            woody->shdr[i].sh_size += woody->payload_size;
    }

    void *payload;
    memcpy(woody->ehdr + text_end, payload, woody->payload_size - jmp_len);

    // Copy the jump at the end of the payload.
    memcpy(woody->ehdr + text_end + woody->payload_size - jmp_len, jmp_entry, jmp_len);
    memcpy(woody->ehdr + text_end + PAGE_SZ64, woody->mmap_ptr + text_end, woody->binary_data_len - text_end);
}