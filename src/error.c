/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:36:46 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:46 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

static void error_msg(int err)
{
    err == ERROR_OPEN
        ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_OPEN\n", err)
        : 0;
    err == ERROR_READ ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_READ\n", err)
                      : 0;
    err == ERROR_WRITE ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_WRITE\n", err)
                       : 0;
    err == ERROR_CLOSE ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_CLOSE\n", err)
                       : 0;
    err == ERROR_MALLOC ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_MALLOC\n", err)
                        : 0;
    err == ERROR_INPUT_ARGUMENTS_NUMBERS ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_INPUT_ARGUMENTS_NUMBERS\n", err)
                                         : 0;
    err == ERROR_LSEEK ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_LSEEK\n", err)
                       : 0;
}

void error(int err, t_woody *woody)
{
    error_msg(err);
    free_woody(woody);
    exit(err);
}