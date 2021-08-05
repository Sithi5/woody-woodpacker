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
    char *code = "\x50\x51\x52\x56\x57\x41\x53\x48\x31\xd2\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\xba\x0d\x00\x00\x00\x48\x8d\x35\x0f\x00\x00\x00\x0f\x05\x41\x5b\x5f\x5e\x5a\x59\x58\x68\x40\x10\x40\x00\xc3\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x0a\x00\x0d\x00\x00";
    
    woody->payload_size = 72;
    if (!(woody->payload_data = (char *)malloc(sizeof(char) * woody->payload_size)))
        error(ERROR_MALLOC, woody);
    memcpy(woody->payload_data,code,woody->payload_size);
    printf("size payload = %ld\n", woody->payload_size);
    printf("payload: \n");
    write(1, (char*)woody->payload_data,63);
    // double payload_size;
    // int fd;

    // if ((fd = open(payload_name, O_RDONLY)) == -1)
    // {
    //     error(ERROR_OPEN, woody);
    // }
    // if ((payload_size = lseek(fd, 0, SEEK_END)) != -1)
    // {
    //     woody->payload_size = (long unsigned int)payload_size;
    //     /* Go back to the start of the file. */
    //     if (lseek(fd, 0, SEEK_SET) != 0)
    //     {
    //         close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
    //     }
    //     if (!(woody->payload_data = malloc(payload_size)))
    //     {
    //         close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_MALLOC, woody);
    //     }
    //     if (read(fd, woody->payload_data, woody->payload_size) == -1)
    //     {
    //         close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_READ, woody);
    //     }
    // }
    // else
    // {
    //     close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
    // }
    // close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
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
    char jump_entry[] = "\x48\xb8\x41\x41\x41\x41\x41\x41\x41\x41" //mov rax,0x4141414141414141
                        "\xff\xe0";                                // jmp rax
    int jump_size = 12;

    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SZ64;

    for (int i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            text_end_offset = woody->phdr[i].p_offset + woody->phdr[i].p_filesz;

            payload_vaddr = woody->phdr[i].p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = payload_vaddr;
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
    memcpy(&jump_entry[2], (char *)&woody->old_entry_point, 8);

    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)text_end_offset);

    memcpy(woody->infected_file + text_end_offset, woody->payload_data, woody->payload_size - jump_size);

    memcpy(woody->infected_file + text_end_offset + woody->payload_size - jump_size, jump_entry, jump_size);

    memcpy(woody->infected_file + text_end_offset + PAGE_SZ64, woody->mmap_ptr + text_end_offset, woody->binary_data_size - text_end_offset);
}