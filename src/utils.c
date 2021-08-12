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
        free(woody);
    }
}

void print_memory(void *memory_ptr, int memory_size)
{
    printf("printing memory at address : %p :\n\n", memory_ptr);
    for (int i = 0; i < memory_size; i++)
    {
        printf("%x ", ((char *)(memory_ptr))[i]);
    }
    printf("\n\n");
}
