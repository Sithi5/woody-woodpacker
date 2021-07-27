/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:29:26 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:29:26 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

/* Free the woody structure properly. */
void free_woody(t_woody *woody)
{
    if (woody)
    {
        woody->file_data ? free(woody->file_data) : 0;
        woody->cipher ? free(woody->cipher) : 0;
        free(woody);
    }
}
