#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

void print_item(FILE *fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[])
{
    int i;
    char *sh_str;
    char *buff;

    buff = malloc(sh_table[eh.e_shstrndx].sh_size);

    if (buff != NULL)
    {
        fseek(fd, sh_table[eh.e_shstrndx].sh_offset, SEEK_SET);
        fread(buff, 1, sh_table[eh.e_shstrndx].sh_size, fd);
    }
    sh_str = buff;

    for (i = 0; i < eh.e_shnum; i++)
    {
        if (!strcmp(".text", (sh_str + sh_table[i].sh_name)))
        {
            printf("Found section\t\".mydata\"\n");
            printf("at offset\t0x%08x\n", (unsigned int)sh_table[i].sh_offset);
            printf("of size\t\t0x%08x\n", (unsigned int)sh_table[i].sh_size);
            break;
        }
    }

    /*Code to print or store string data*/
    if (i < eh.e_shnum)
    {
        char *mydata = malloc(sh_table[i].sh_size);
        fseek(fd, sh_table[i].sh_offset, SEEK_SET);
        fread(mydata, 1, sh_table[i].sh_size, fd);
        puts(mydata);
    }
    else
    {
        // .mydata section not found
    }
}

int main(int ac, char **av)
{
    FILE *fp = NULL; //Pointer used to access current file
    char *program_name;
    Elf64_Ehdr elf_header; //Elf header
    Elf64_Shdr *sh_table;  //Elf symbol table

    program_name = av[1];
    fp = fopen(program_name, "r");

    fseek(fp, 0, SEEK_SET);
    fread(&elf_header, 1, sizeof(Elf64_Ehdr), fp);
    sh_table = malloc(elf_header.e_shentsize * elf_header.e_shnum);

    fseek(fp, elf_header.e_shoff, SEEK_SET);
    fread(sh_table, 1, elf_header.e_shentsize * elf_header.e_shnum, fp);

    print_item(fp, elf_header, sh_table);

    return 0;
}