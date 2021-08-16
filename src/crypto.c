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
    char *key = "0";

    if (!(woody->cipher = (char *)malloc(sizeof(char) * text_len)))
        error(ERROR_MALLOC, woody);
    memcpy(woody->cipher, woody->mmap_ptr + woody->text_start_offset, text_len);
    //woody->cipher = XORCipher(woody->mmap_ptr + woody->text_start_offset, key, text_len, strlen(key));
    rc4_cipher_start(woody->cipher,text_len,key,strlen(key));
}