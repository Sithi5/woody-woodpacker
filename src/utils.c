#include "woody_woodpacker.h"

/* Free the woody structure properly. */
void free_woody(t_woody *woody)
{
    if (woody)
    {
        woody->file_data ? free(woody->file_data) : 0;
        woody->cipher ? free(woody->cipher) : 0;
        free(woody);
    }
}
