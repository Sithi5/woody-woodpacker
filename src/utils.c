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
    printf("Offsets:\n");
    printf("\twoody->text_s_offset:\t\t\t\t%lu\n", woody->text_s_offset);
    printf("\twoody->text_s_size:\t\t\t\t%lu\n", woody->text_s_size);
    printf("\twoody->encrypt_start_offset:\t\t\t%lu\n", woody->encrypt_start_offset);
    printf("\twoody->encrypt_end_offset:\t\t\t%lu\n", woody->encrypt_end_offset);
    printf("Adresses:\n");
    printf("\twoody->old_entry_point:\t\t\t\t0x%lx -> %lu\n", woody->old_entry_point, woody->old_entry_point);
    printf("\twoody->new_entry_point:\t\t\t\t0x%lx -> %lu\n", woody->new_entry_point, woody->new_entry_point);
    printf("\twoody->text_p_vaddr:\t\t\t\t0x%lx -> %lu\n", woody->text_p_vaddr, woody->text_p_vaddr);
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
