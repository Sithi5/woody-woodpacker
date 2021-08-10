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

/* Simple XOR encryption/decryption algorithm. */
static char *XORCipher(char *data, char *key, int data_len, int key_len)
{
    char *output = (char *)malloc(sizeof(char) * data_len);

    for (int i = 0; i < data_len; ++i)
    {
        output[i] = data[i] ^ key[i % key_len];
        //output[i] = data[i] ^ 0x54;
    }

    return output;
}

void cipher_woody_file_data(t_woody *woody)
{
    size_t text_len;
    text_len = woody->elf64_ptrs->text_end_offset - woody->elf64_ptrs->text_start_offset;
    char *key = "0";

    printf("uncrypted data: \n");
    for (int i = 0; i < text_len; i++)
    {
        printf("%#x ", ((unsigned char *)(woody->mmap_ptr))[i]);
    }
    printf("\n");
    printf("C XOR CIPHER\n");
    woody->cipher = XORCipher(woody->mmap_ptr + woody->elf64_ptrs->text_start_offset, key, text_len, strlen(key));

    //asm TEST
    printf("ASM XOR CIPHER\n");
    printf("binarylen %d\n", text_len);
    extern char *asmxorcipher(void *data, char *key, int datalen, int keylen);
    void *crypt;
    void *decrypt;
    crypt = asmxorcipher(woody->mmap_ptr + woody->elf64_ptrs->text_start_offset, key, text_len, strlen(key));
    printf("crypt lenght = %ld\n\n", text_len);
    printf("DIFF: %d\n", memcmp(woody->cipher, crypt, text_len));
    decrypt = asmxorcipher(crypt, key, text_len, strlen(key));
    printf("crypted data: \n");
    for (int i = 0; i < text_len; i++)
    {
        printf("%#x ", ((unsigned char *)(woody->cipher))[i]);
    }
}