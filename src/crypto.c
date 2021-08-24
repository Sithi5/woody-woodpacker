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
    if (!(woody->cipher = (void *)malloc(woody->encrypt_s_size)))
        error(ERROR_MALLOC, woody);
    memcpy(woody->cipher, woody->mmap_ptr + woody->encrypt_s_start_offset, woody->encrypt_s_size);

    key_generator(woody);
    if (ARCH_32)
    {
        printf("ARCH 32 CIPHER\n");
        // asm_xor_cipher(woody->cipher, woody->encrypt_s_size, woody->encryption_key, KEY_LEN);
    }
    else if (ARCH_64)
    {
        printf("ARCH 64 CIPHER\n");
        // rc4_cipher_start(woody->cipher, woody->encrypt_s_size, woody->encryption_key, KEY_LEN);
    }
}