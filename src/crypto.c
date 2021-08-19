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
    size_t text_len;
    text_len = woody->text_end_offset - woody->text_start_offset;
    if (!(woody->cipher = (void *)malloc(text_len)))
        error(ERROR_MALLOC, woody);
    memcpy(woody->cipher, woody->mmap_ptr + woody->text_start_offset, text_len);

    printf("testing xor cipher : \n\n");
    char key[4] = "KEY";
    char cipher[10] = "Bonjourln";
    printf("key : %s\n", key);
    printf("cipher : %s\n", cipher);
    asm_xor_cipher(cipher, strlen(cipher), key, strlen(key));
    printf("\nafter\n\n");
    printf("key : %s\n", key);
    printf("cipher : %s\n", cipher);
    printf("\n");

    // key_generator(woody);
    // if (ARCH_32)
    // {
    //     printf("ARCH 32 CIPHER\n");
    //     asm_xor_cipher(woody->cipher, text_len, woody->encryption_key, KEY_LEN);
    // }
    // else
    // {
    //     printf("ARCH 64 CIPHER\n");
    //     rc4_cipher_start(woody->cipher, text_len, woody->encryption_key, KEY_LEN);
    // }
}