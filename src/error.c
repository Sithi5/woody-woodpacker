/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:29:29 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:29:29 by mabouce          ###   ########.fr       */
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
    err == ERROR_NOT_ELF ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_NOT_ELF\n", err)
                         : 0;
    err == ERROR_NOT_ELF32 ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_NOT_ELF32\n", err)
                           : 0;
    err == ERROR_NOT_ELF64 ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_NOT_ELF64\n", err)
                           : 0;
    err == ERROR_MMAP ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_MMAP\n", err)
                      : 0;
    err == ERROR_NOT_EXECUTABLE_BINARY ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_NOT_EXECUTABLE_BINARY\n", err)
                                       : 0;
    err == ERROR_ELF_NOT_LITTLE_ENDIAN ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_ELF_NOT_LITTLE_ENDIAN\n", err)
                                       : 0;
    err == ERROR_RET2OEP_NOT_FOUND ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_RET2OEP_NOT_FOUND\n", err)
                                   : 0;
    err == ERROR_FILE_IS_ALREADY_INFECTED ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_FILE_IS_ALREADY_INFECTED\n", err)
                                          : 0;
    err == ERROR_PAYLOAD_TOO_BIG ? fprintf(stderr, RED "error" DEF ": %d -> ERROR_PAYLOAD_TOO_BIG\n", err)
                                 : 0;
    if (errno != 0)
    {
        fprintf(stderr, RED "errno " DEF ": %d -> %s\n", errno, strerror(errno));
    }
}

void error(int err, t_woody *woody)
{
    error_msg(err);
    free_woody(woody);
    exit(err);
}