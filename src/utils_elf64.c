/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_elf64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 15:58:26 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/28 15:58:26 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

Elf64_Off get_padding_size(t_woody *woody, Elf64_Ehdr *header)
{
    // int pt_entry_count = header->e_phnum;

    // Elf64_Phdr *prog_header_ptr_entry = (Elf64_Phdr *)(woody->mmap_ptr + header->e_phoff);
    // bool CODE_SEGMENT_FOUND = false;
    // int i = 0;
    // while (i < pt_entry_count)
    // {
    //     if (CODE_SEGMENT_FOUND == false &&
    //         prog_header_ptr_entry->p_type == PT_LOAD &&
    //         prog_header_ptr_entry->p_flag == (PF_R || PF_X))
    //     {
    //         CODE_SEGMENT_FOUND = true;
    //         woody->segment_end_offset = prog_header_ptr_entry->p_offset + prog_header_ptr_entry->p_filesz;
    //         woody->woody_offset = woody->segment_end_offset;
    //         woody->woody_load_address = prog_header_ptr_entry->p_vaddr + prog_header_ptr_entry->filesz;
    //         prog_header_ptr_entry->p_filesz += woody->woody_size;
    //         prog_header_ptr_entry->p_memsz += woody->woody_size;
    //     }
    //     if (CODE_SEGMENT_FOUND == true &&
    //         prog_header_ptr_entry->p_type == PT_LOAD &&
    //         prog_header_ptr_entry->p_flag == (PF_R || PF_W)) )
    //         return (prog_header_ptr_entry->p_offset - woody->woody_offset);
    //     i++;
    //     ++prog_header_ptr_entry;
    // }
}