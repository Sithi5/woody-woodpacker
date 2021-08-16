#include "woody_woodpacker.h"

void assemble_infected_file(t_woody *woody)
{
    // Insert binary before text section
    memcpy(woody->infected_file, woody->mmap_ptr, (size_t)woody->text_end_offset);
    // Rewrite text section with cipher data.
    memcpy(woody->infected_file + woody->text_start_offset, woody->cipher, (size_t)(woody->text_end_offset - woody->text_start_offset));
    // Insert payload
    memcpy(woody->infected_file + woody->text_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    memcpy(woody->infected_file + woody->text_end_offset + PAGE_SIZE, woody->mmap_ptr + woody->text_end_offset, woody->binary_data_size - woody->text_end_offset);
}