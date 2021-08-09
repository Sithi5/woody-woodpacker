/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect_elf_32.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 15:08:25 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/09 15:08:25 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void infect_elf_32(t_woody *woody)
{
    if (!(woody->elf32_ptrs = (t_elf32_ptrs *)malloc(sizeof(t_elf32_ptrs))))
        error(ERROR_MALLOC, woody);
}