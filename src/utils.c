/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:36:50 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:50 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void set_woody_ptrs_to_null(t_woody *woody)
{
    woody->mmap_ptr = NULL;
    woody->payload_data = NULL;
    woody->infected_file = NULL;
    woody->cipher = NULL;
    woody->encryption_key = NULL;
}

/* Free the woody structure properly. */
void free_woody(t_woody *woody)
{
    if (woody)
    {
        woody->mmap_ptr ? munmap(woody->mmap_ptr, woody->binary_data_size) : 0;
        woody->payload_data ? free(woody->payload_data) : 0;
        woody->infected_file ? free(woody->infected_file) : 0;
        woody->cipher ? free(woody->cipher) : 0;
        woody->encryption_key ? free(woody->encryption_key) : 0;
        free(woody);
    }
}

void print_woody_infos(t_woody *woody)
{
    printf("\n");
    printf("Elf ehdr:\n");
    printf("\twoody->ehdr->e_phoff:\t\t\t\t%u\n", woody->ehdr->e_phoff);       /* Program header table file offset */
    printf("\twoody->ehdr->e_shoff:\t\t\t\t%u\n", woody->ehdr->e_shoff);       /* Section header table file offset */
    printf("\twoody->ehdr->e_flags:\t\t\t\t%u\n", woody->ehdr->e_flags);       /* Processor-specific flags */
    printf("\twoody->ehdr->e_ehsize:\t\t\t\t%u\n", woody->ehdr->e_ehsize);     /* ELF header size in bytes */
    printf("\twoody->ehdr->e_phentsize:\t\t\t%u\n", woody->ehdr->e_phentsize); /* Program header table entry size */
    printf("\twoody->ehdr->e_phnum:\t\t\t\t%u\n", woody->ehdr->e_phnum);       /* Program header table entry count */
    printf("\twoody->ehdr->e_shentsize:\t\t\t%u\n", woody->ehdr->e_shentsize); /* Section header table entry size */
    printf("\twoody->ehdr->e_shnum:\t\t\t\t%u\n", woody->ehdr->e_shnum);       /* Section header table entry count */
    printf("\twoody->ehdr->e_shstrndx:\t\t\t%u\n", woody->ehdr->e_shstrndx);   /* Section header string table index */
    printf("Offsets:\n");
    printf("\twoody->text_p_start_offset:\t\t\t%lu\n", woody->text_p_start_offset);
    printf("\twoody->text_p_end_offset:\t\t\t%lu\n", woody->text_p_end_offset);
    printf("\twoody->encrypt_s_start_offset:\t\t\t%lu\n", woody->encrypt_s_start_offset);
    printf("\twoody->encrypt_s_end_offset:\t\t\t%lu\n", woody->encrypt_s_end_offset);
    printf("Adresses:\n");
    printf("\twoody->old_entry_point:\t\t\t\t0x%lx -> %lu\n", woody->old_entry_point, woody->old_entry_point);
    printf("\twoody->new_entry_point:\t\t\t\t0x%lx -> %lu\n", woody->new_entry_point, woody->new_entry_point);
    printf("\twoody->text_p_vaddr:\t\t\t\t0x%lx -> %lu\n", woody->text_p_vaddr, woody->text_p_vaddr);
    printf("\twoody->encrypt_s_addr:\t\t\t\t0x%lx -> %lu\n", woody->encrypt_s_addr, woody->encrypt_s_addr);
    printf("\n");
}

void print_memory(void *memory_ptr, size_t memory_size)
{
    printf("printing memory at address : %p :\n\n", memory_ptr);
    for (size_t i = 0; i < memory_size; i++)
    {
        printf("%x ", ((char *)(memory_ptr))[i]);
    }
    printf("\n\n");
}
