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
	}

	return output;
}

void cipher_woody_file_data(t_woody *woody)
{
	woody->cipher = XORCipher(woody->file_data, "MyXorKey", woody->file_data_len, 5);
	printf("Encryption Key : MyXorKey\n");
}