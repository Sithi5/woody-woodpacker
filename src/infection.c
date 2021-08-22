/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_infection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:10:16 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/18 14:10:16 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void choose_infection_method(t_woody *woody)
{
    set_string_table_ptr(woody);
    load_payload(woody, PAYLOAD_NAME);

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->text_p_start_offset = woody->phdr[i].p_offset;
            woody->text_p_end_offset = woody->phdr[i].p_offset + woody->phdr[i].p_filesz;
            woody->text_p_size = woody->phdr[i].p_filesz;

            // Check if there is enought space for our payload in the text section.
            if (woody->text_p_end_offset % PAGE_SIZE + woody->payload_size < PAGE_SIZE)
            {
                printf("USING SILVIO INFECTION\n");
                silvio_text_infection(woody);
            }
            else
            {
                printf("USING PT_NOTE_TO_PT_LOAD INFECTION\n");
                pt_note_to_pt_load_infection(woody);
            }
            break;
        }
    }
}