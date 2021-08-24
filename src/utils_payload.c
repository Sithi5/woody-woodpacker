/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_payload.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 15:42:33 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/09 15:42:33 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

size_t find_keysection_offset(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x44)
        {
            if (woody->payload_size - i > 128)
            {
                if (((char *)woody->payload_data)[i + 1] == 0x44 && ((char *)woody->payload_data)[i + 2] == 0x44 && ((char *)woody->payload_data)[i + 3] == 0x44)
                {
                    return i;
                }
            }
        }
    }
    error(ERROR_KEYSECTION_NOT_FOUND, woody);
    return -1;
}

// Find the ret2oep offset in the payload. return true if ret2oep have been found.
size_t find_ret2oep_offset_32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x77)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2oep
                if (((char *)woody->payload_data)[i + 1] == 0x77 && ((char *)woody->payload_data)[i + 2] == 0x77 &&
                    ((char *)woody->payload_data)[i + 3] == 0x77 &&
                    ((char *)woody->payload_data)[i + 4] == 0x2d &&
                    ((char *)woody->payload_data)[i + 5] == 0x77 && ((char *)woody->payload_data)[i + 6] == 0x77 &&
                    ((char *)woody->payload_data)[i + 7] == 0x77 && ((char *)woody->payload_data)[i + 8] == 0x77 &&
                    ((char *)woody->payload_data)[i + 9] == 0x05 &&
                    ((char *)woody->payload_data)[i + 10] == 0x77 && ((char *)woody->payload_data)[i + 11] == 0x77 &&
                    ((char *)woody->payload_data)[i + 12] == 0x77 && ((char *)woody->payload_data)[i + 13] == 0x77)
                {
                    // Removing 2 to go to actual start of ret2oep (go back to instructions sub).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_RET2OEP_NOT_FOUND, woody);
    return -1;
}

// Find the ret2oep offset in the payload. return true if ret2oep have been found.
size_t find_ret2oep_offset(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x77)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2oep
                if (((char *)woody->payload_data)[i + 1] == 0x77 && ((char *)woody->payload_data)[i + 2] == 0x77 &&
                    ((char *)woody->payload_data)[i + 3] == 0x77 &&
                    ((char *)woody->payload_data)[i + 4] == 0x48 && ((char *)woody->payload_data)[i + 5] == 0x2d &&
                    ((char *)woody->payload_data)[i + 6] == 0x77 && ((char *)woody->payload_data)[i + 7] == 0x77 &&
                    ((char *)woody->payload_data)[i + 8] == 0x77 && ((char *)woody->payload_data)[i + 9] == 0x77 &&
                    ((char *)woody->payload_data)[i + 10] == 0x48 && ((char *)woody->payload_data)[i + 11] == 0x05 &&
                    ((char *)woody->payload_data)[i + 12] == 0x77 && ((char *)woody->payload_data)[i + 13] == 0x77 &&
                    ((char *)woody->payload_data)[i + 14] == 0x77 && ((char *)woody->payload_data)[i + 15] == 0x77)
                {
                    // Removing 2 to go to actual start of ret2oep (go back to instructions sub).
                    return i - 2;
                }
            }
        }
    }
    error(ERROR_RET2OEP_NOT_FOUND, woody);
    return -1;
}

// Find the ret2textsection offset in the payload. return true if ret2textsection have been found.
size_t find_getencryptedsectionaddr_offset_elf64(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x66)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2textsection
                if (((char *)woody->payload_data)[i + 1] == 0x66 && ((char *)woody->payload_data)[i + 2] == 0x66 &&
                    ((char *)woody->payload_data)[i + 3] == 0x66 &&
                    ((char *)woody->payload_data)[i + 4] == 0x48 && ((char *)woody->payload_data)[i + 5] == 0x2d &&
                    ((char *)woody->payload_data)[i + 6] == 0x66 && ((char *)woody->payload_data)[i + 7] == 0x66 &&
                    ((char *)woody->payload_data)[i + 8] == 0x66 && ((char *)woody->payload_data)[i + 9] == 0x66 &&
                    ((char *)woody->payload_data)[i + 10] == 0x48 && ((char *)woody->payload_data)[i + 11] == 0x05 &&
                    ((char *)woody->payload_data)[i + 12] == 0x66 && ((char *)woody->payload_data)[i + 13] == 0x66 &&
                    ((char *)woody->payload_data)[i + 14] == 0x66 && ((char *)woody->payload_data)[i + 15] == 0x66)
                {
                    // Removing 2 to go to actual start of ret2textsection (go back to instructions sub).
                    return i - 2;
                }
            }
        }
    }
    error(ERROR_RET2TEXTSECTION_NOT_FOUND, woody);
    return -1;
}

// Find the getencryptedsectionsize offset in the payload. return true if getencryptedsectionsize have been found.
size_t find_getencryptedsectionsize_offset_elf64(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x55)
        {
            if (woody->payload_size - i > 4)
            {
                if (((char *)woody->payload_data)[i + 1] == 0x55 &&
                    ((char *)woody->payload_data)[i + 2] == 0x55 &&
                    ((char *)woody->payload_data)[i + 3] == 0x55)
                {
                    // Removing 2 to go to actual start of getencryptedsectionsize (go back to instructions mov).
                    return i - 2;
                }
            }
        }
    }
    error(ERROR_SETTEXTSECTIONSIZE_NOT_FOUND, woody);
    return -1;
}

// Rewrite info in payload ret2oep.
void overwrite_payload_ret2oep(t_woody *woody)
{
    size_t ret2oep_offset;
    if (ARCH_32)
    {
        ret2oep_offset = find_ret2oep_offset_32(woody);
        // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
        memcpy(woody->payload_data + ret2oep_offset + 1, (void *)(&(ret2oep_offset)), 4);
        // Rewrite new entry_point in payload ret2oep.
        memcpy(woody->payload_data + ret2oep_offset + 6, (void *)&(woody->new_entry_point), 4);
        // Rewrite old entry_point in payload ret2oep.
        memcpy(woody->payload_data + ret2oep_offset + 11, (void *)&(woody->old_entry_point), 4);
    }
    else if (ARCH_64)
    {
        ret2oep_offset = find_ret2oep_offset(woody);
        // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
        memcpy(woody->payload_data + ret2oep_offset + 2, (void *)(&(ret2oep_offset)), 4);
        // Rewrite new entry_point in payload ret2oep.
        memcpy(woody->payload_data + ret2oep_offset + 8, (void *)&(woody->new_entry_point), 4);
        // Rewrite old entry_point in payload ret2oep.
        memcpy(woody->payload_data + ret2oep_offset + 14, (void *)&(woody->old_entry_point), 4);
    }
}

// Rewrite info in payload getencryptedsectionaddr.
void overwrite_payload_getencryptedsectionaddr(t_woody *woody)
{
    size_t getencryptedsectionaddr_offset = find_getencryptedsectionaddr_offset_elf64(woody);
    // Rewrite payload size without getencryptedsectionaddr. + 2 to skip two first instructions and go to address.
    memcpy(woody->payload_data + getencryptedsectionaddr_offset + 2, (void *)(&(getencryptedsectionaddr_offset)), 4);
    // Rewrite new entry_point in payload getencryptedsectionaddr.
    memcpy(woody->payload_data + getencryptedsectionaddr_offset + 8, (void *)&(woody->new_entry_point), 4);
    // Rewrite old entry_point in payload getencryptedsectionaddr.
    memcpy(woody->payload_data + getencryptedsectionaddr_offset + 14, (void *)&(woody->encrypt_s_addr), 4);
}

// Rewrite info in payload getencryptedsectionsize.
void overwrite_payload_getencryptedsectionsize(t_woody *woody)
{
    size_t getencryptedsectionsize_offset = find_getencryptedsectionsize_offset_elf64(woody);
    // Rewrite getencryptedsectionsize_offset + 2 to skip two first instructions and go to textoffset value.
    memcpy(woody->payload_data + getencryptedsectionsize_offset + 2, (void *)&(woody->encrypt_s_size), 4);
}

void overwrite_keysection_payload(t_woody *woody)
{
    size_t keysection_offset = find_keysection_offset(woody);
    memcpy(woody->payload_data + keysection_offset, woody->encryption_key, KEY_LEN);
}

void load_payload(t_woody *woody, char *payload_name)
{
    double payload_size;
    int fd;

    if ((fd = open(payload_name, O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if ((payload_size = lseek(fd, 0, SEEK_END)) != -1)
    {
        woody->payload_size = (size_t)payload_size;
        /* Go back to the start of the file. */
        if (lseek(fd, 0, SEEK_SET) != 0)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
        }
        if (!(woody->payload_data = malloc(payload_size)))
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_MALLOC, woody);
        }
        if (read(fd, woody->payload_data, woody->payload_size) == -1)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_READ, woody);
        }
    }
    else
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
}