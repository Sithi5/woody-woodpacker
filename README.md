# woody-woodpacker
Projet qui a pour principe de modifier les headers d'un fichier de type ELF64. Le but ici est de pouvoir ajouter un morceau de code et obfusquer une partie d'un fichier non strippé.

# Usefull commands
```bash
readelf -h # Display elf-header information for binaries files
readelf --symbol # Display symbol information for binaries files
readelf -l # Display program-header info
```

# Resources
* [elf.h](https://code.woboq.org/linux/include/elf.h.html)
* [UNIX ELF Parasites and virus](https://ivanlef0u.fr/repo/madchat/vxdevl/vdat/tuunix02.htm)
* [Malware Engineering Part 0x2— Finding shelter for parasite](https://medium.com/analytics-vidhya/malware-engineering-part-0x2-finding-shelter-for-parasite-751145dd18d0)
* [Linux x64 Infection for Lamers (by a Lamer)](https://vx-underground.org/archive/VxHeaven/lib/vjp01.html)
* [Return To Original Entry Point Despite PIE](https://tmpout.sh/1/11.html)
* [dcode.fr/rc4-cipher](https://www.dcode.fr/rc4-cipher)
* [wikipedia.org/wiki/RC4](https://fr.wikipedia.org/wiki/RC4)
* [geeksforgeeks.org/implementation-of-rc4-algorithm](https://www.geeksforgeeks.org/implementation-of-rc4-algorithm/)

# Build and run
```bash
make all
./woody-woopacker (binary_file)
./woody
```

# Testing
```bash
make tests
```
