/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_woody_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:29:23 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:29:23 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void write_woody_file(t_woody *woody)
{
    int fd;
    if ((fd = open("woody", O_WRONLY | O_CREAT, S_IRWXU)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if ((write(fd, woody->file_data, woody->file_data_len)) == -1)
    {
        error(ERROR_WRITE, woody);
    }
    if ((close(fd)) == -1)
    {
        error(ERROR_CLOSE, woody);
    }
}