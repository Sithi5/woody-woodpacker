/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypto.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:36:43 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:43 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void cipher_woody_file_data(t_woody *woody)
{
    woody->cipher_size = woody->encrypt_s_size;
    printf("woody->cipher_size = %u\n", woody->cipher_size);
    printf("woody->encrypt_s_start_offset = %u\n", woody->encrypt_s_start_offset);
    if (!(woody->cipher = (void *)malloc(woody->cipher_size)))
        error(ERROR_MALLOC, woody);
    memcpy(woody->cipher, woody->mmap_ptr + woody->encrypt_s_start_offset, woody->cipher_size);

    key_generator(woody);
    if (ARCH_32)
    {
        printf("ARCH 32 CIPHER\n");
        // asm_xor_cipher(woody->cipher, woody->cipher_size, woody->encryption_key, KEY_LEN);
    }
    else if (ARCH_64)
    {
        printf("ARCH 64 CIPHER\n");
        rc4_cipher_start(woody->cipher, woody->cipher_size, woody->encryption_key, KEY_LEN);
    }
}