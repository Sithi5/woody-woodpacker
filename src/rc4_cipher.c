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

void ft_swap(char *a, char *b)
{
    char tmp = *b;
    *a = *b;
    *b = tmp;
}

void encryption(char *stream, char *result, char *data, int len)
{
    int i = 0, j = 0, res = 0;

    for (int k = 0; k < len; k++)
    {
        i = (i + 1) % N;
        j = (j + stream[i]) % N;
        ft_swap(&stream[i], &stream[j]);
        res = stream[(stream[i] + stream[j]) % N];
        result[k] = res ^ data[k];
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
        ft_swap(&stream[i], &stream[j]);
    }
}

char *random_string(char *key, int len, t_woody *woody)
{
    int fd;

    if ((fd = open("/dev/random", O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if (read(fd, key, len) == -1)
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_READ, woody);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
    return (key);
}

char *rc4_cipher(t_woody *woody, char *data, int len)
{
    char *stream;
    char *result;

    if (!(stream = (char *)malloc(sizeof(char) * N)))
        error(ERROR_MALLOC, woody);
    if (!(result = (char *)malloc(sizeof(char) * len)))
        error(ERROR_MALLOC, woody);
    if (!(woody->encryption_key = (char *)malloc(sizeof(char) * 40)))
        error(ERROR_MALLOC, woody);
    woody->encryption_key = random_string(woody->encryption_key, 40, woody);
    Keystream_generation(stream, woody->encryption_key);
    printf("encryption key = ");
    for (int i = 0; i < 40; i++)
        printf("%c", woody->encryption_key[i]);
    printf("\n\n");
    encryption(stream, result, data, len);
    printf("result = ");
    for (int i = 0; i < len; i++)
        printf("%x", result[i]);
    return result;
}