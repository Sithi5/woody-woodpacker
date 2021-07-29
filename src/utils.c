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

/* Free the woody structure properly. */
void free_woody(t_woody *woody)
{
    if (woody)
    {
        woody->mmap_ptr ? munmap(woody->mmap_ptr, woody->binary_data_len) : 0;
        free(woody);
    }
}
