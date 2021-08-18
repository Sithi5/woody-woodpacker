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
    load_payload(woody, PAYLOAD_NAME);

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->text_start_offset = woody->phdr[i].p_offset;
            woody->text_end_offset = woody->text_start_offset + woody->phdr[i].p_filesz;
            woody->text_section_size = woody->phdr[i].p_filesz;

            cipher_woody_file_data(woody);

            pt_note_to_pt_load_infection(woody);
            break;
            // Check if there is enought space for our payload in the text section.
            if (woody->text_end_offset % PAGE_SIZE + woody->payload_size < PAGE_SIZE)
            {
                silvio_text_infection(woody);
            }
            else
            {
                pt_note_to_pt_load_infection(woody);
            }
            break;
        }
    }
}