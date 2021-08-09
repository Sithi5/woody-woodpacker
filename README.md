# woody-woodpacker
Projet qui a pour principe de modifier les headers d'un fichier de type ELF64. Le but ici est de pouvoir ajouter un morceau de code et obfusquer une partie d'un fichier non strippé.

# Usefull commands
* `readelf -h` : display elf-header information for binaries files
* `readelf --symbol` : display symbol information for binaries files
* `readelf -l`

# Resources
* [elf.h](https://code.woboq.org/linux/include/elf.h.html)
* [UNIX ELF Parasites and virus](https://ivanlef0u.fr/repo/madchat/vxdevl/vdat/tuunix02.htm)
* [Malware Engineering Part 0x2— Finding shelter for parasite](https://medium.com/analytics-vidhya/malware-engineering-part-0x2-finding-shelter-for-parasite-751145dd18d0)
* [Linux x64 Infection for Lamers (by a Lamer)](https://vx-underground.org/archive/VxHeaven/lib/vjp01.html)
* [Return To Original Entry Point Despite PIE](https://tmpout.sh/1/11.html)

# Testing
`sh scripts/simple_testing_woody.sh`