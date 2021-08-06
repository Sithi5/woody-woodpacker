/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc4_cipher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maginist <maginist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 15:32:34 by maginist          #+#    #+#             */
/*   Updated: 2021/08/06 15:32:34 by maginist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

#define N 256

void swap(char *a, char *b)
{
    char tmp = *b;
    *a = *b;
    *b = tmp;
}

void encryption(char *stream, t_woody *woody)
{
    int i = 0, j = 0;

    for (int k = 0; k < woody->text_section_size; k++)
    {
        i = (i + 1) % N;
        j = (j + stream[i]) % N;
        swap(&stream[i], &stream[j]);
        int res = stream[(stream[i] + stream[j]) % N];
        woody->encryption_result[k] = res ^ woody->encryption_data[k];
    }
}

void Keystream_generation(char *stream, char *key)
{
    int j = 0;
    for (int i = 0; i < N; i++)
    {
        stream[i] = i;
    }
    for (int i = 0; i < N; i++)
    {
        j = (j + stream[i] + key[i % 40]) % N;
        swap(&stream[i], &stream[j]);
    }
}

char *random_string(char *key, int len)
{
    srand(time(NULL));
    for (int i = 0; i < len; ++i)
    {
        key[i] = '0' + rand() % 72;
    }
    return (key);
}

void rc4_cipher(t_woody *woody)
{
    char *stream;

    if (!(stream = (char *)malloc(sizeof(char) * N)))
        error(ERROR_MALLOC, woody);
    if (!(woody->encryption_key = (char *)malloc(sizeof(char) * 40)))
        error(ERROR_MALLOC, woody);
    woody->encryption_key = random_string(woody->encryption_key, 40);
    Keystream_generation(stream, woody->encryption_key);
    printf("encryption key = ");
    for (int i = 0; i < 40; i++)
        printf("%c", woody->encryption_key[i]);
    printf("\n");
    encryption(stream, woody);
}