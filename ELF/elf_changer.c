#include <elf.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "load_elf.c"
#include <fcntl.h>
extern int optind;
extern char *optarg;

void save_changes(char *elf_path, char *elf_start, off_t file_size) {
    int fd = open(elf_path, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    if (write(fd, elf_start, file_size) != file_size) {
        perror("write");
        close(fd);
        return;
    }
    close(fd);
}

void elf_27255_glava(char *elf_start) {
    // Kazalec na začetek ELF datoteke pretvorimo v kazalec na strukturo Elf32_Ehdr
    Elf32_Ehdr *header = (Elf32_Ehdr *)elf_start;

    // Izpišemo glavo ELF datoteke
    printf("ELF Header:\n");
    // Izpišemo magično številko ELF datoteke
    printf("  Magic number:                      %.2x %.2x %.2x %.2x\n", header->e_ident[EI_MAG0], header->e_ident[EI_MAG1], header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
    // Izpišemo razred ELF datoteke (32-bitni ali 64-bitni)
    printf("  Class:                             %d-bit object\n", header->e_ident[EI_CLASS] == ELFCLASS32 ? 32 : 64);
    // Izpišemo podatkovno obliko ELF datoteke (mali ali veliki konec)
    printf("  Data:                              %s\n", header->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" : "2's complement, big endian");
    // Izpišemo verzijo ELF datoteke
    printf("  Version:                           %d (current)\n", header->e_ident[EI_VERSION]);
    // Izpišemo ABI ELF datoteke (vedno UNIX System V)
    printf("  OS ABI:                            UNIX System V\n");
    // Izpišemo verzijo ABI ELF datoteke
    printf("  ABI version:                       %d\n", header->e_ident[EI_OSABI]);
    // Izpišemo tip ELF datoteke (izvršljiva ali neznano)
    printf("  Type:                              %s\n", header->e_type == ET_EXEC ? "Executable file" : "Unknown");
    // Izpišemo strojno arhitekturo ELF datoteke (Intel 80386 ali neznano)
    printf("  Machine:                           %s\n", header->e_machine == EM_386 ? "Intel 80386" : "Unknown");
    // Izpišemo verzijo ELF datoteke
    printf("  Version:                           %d (current)\n", header->e_version);
    // Izpišemo naslov vstopne točke ELF datoteke
    printf("  Entry point address:               0x%x\n", header->e_entry);
    // Izpišemo začetek programskih glav ELF datoteke
    printf("  Start of program headers:          %d\n", header->e_phoff);
    // Izpišemo začetek sekcij glav ELF datoteke
    printf("  Start of section headers:          %d\n", header->e_shoff);
    // Izpišemo zastavice ELF datoteke
    printf("  Flags:                             0x%x\n", header->e_flags);
    // Izpišemo velikost glave ELF datoteke
    printf("  Size of this header:               %d B\n", header->e_ehsize);
    // Izpišemo velikost programskih glav ELF datoteke
    printf("  Size of program headers:           %d B\n", header->e_phentsize);
    // Izpišemo število programskih glav ELF datoteke
    printf("  Number of program headers:         %d\n", header->e_phnum);
    // Izpišemo velikost sekcij glav ELF datoteke
    printf("  Size of section headers:           %d B\n", header->e_shentsize);
    // Izpišemo število sekcij glav ELF datoteke
    printf("  Number of section headers:         %d\n", header->e_shnum);
    // Izpišemo indeks nizov sekcij glav ELF datoteke
    printf("  Section header string table index: %d\n", header->e_shstrndx);
}


void elf_27255_simboli(char *elf_start) {
    // Kazalec na začetek ELF datoteke pretvorimo v kazalec na strukturo Elf32_Ehdr
    Elf32_Ehdr *header = (Elf32_Ehdr *)elf_start;
    // Izračunamo kazalec na začetek sekcij v ELF datoteki
    Elf32_Shdr *sections = (Elf32_Shdr *)(elf_start + header->e_shoff);
    // Ustvarimo kazalec na sekcijo simbolne tabele, ki je trenutno NULL
    Elf32_Shdr *symtab_section = NULL;
    // Ustvarimo kazalec na simbolno tabelo, ki je trenutno NULL
    Elf32_Sym *symtab = NULL;
    // Ustvarimo spremenljivko za štetje
    Elf32_Word i;
    // Ustvarimo kazalec na nizovno tabelo
    char *strtab;

    // Poiščemo simbolno tabelo
    for (i = 0; i < header->e_shnum; i++) {
        // Če je tip trenutne sekcije simbolna tabela, shranimo kazalec na to sekcijo in prekinemo zanko
        if (sections[i].sh_type == SHT_SYMTAB) {
            symtab_section = &sections[i];
            break;
        }
    }

    // Če nismo našli simbolne tabele, izpišemo napako in končamo funkcijo
    if (symtab_section == NULL) {
        printf("No symbol table found.\n");
        return;
    }

    // Pridobimo simbolno tabelo in nizovno tabelo
    symtab = (Elf32_Sym *)(elf_start + symtab_section->sh_offset);
    strtab = elf_start + sections[symtab_section->sh_link].sh_offset;

    // Izpišemo vse simbole, ki so večji od 4 zlogov
    for (i = 0; i < symtab_section->sh_size / sizeof(Elf32_Sym); i++) {
        if (symtab[i].st_size > 4) {
            printf("  [%02d]   0x%08x      %d      %s\n", i, symtab[i].st_value, symtab[i].st_size, strtab + symtab[i].st_name);
        }
    }
}

void elf_27255_menjaj(char *elf_start, char **variables, int var_count) {
    // Kazalec na začetek ELF datoteke pretvorimo v kazalec na strukturo Elf32_Ehdr
    Elf32_Ehdr *header = (Elf32_Ehdr *)elf_start;
    // Izračunamo kazalec na začetek sekcij v ELF datoteki
    Elf32_Shdr *sections = (Elf32_Shdr *)(elf_start + header->e_shoff);
    // Ustvarimo kazalec na sekcijo simbolne tabele, ki je trenutno NULL
    Elf32_Shdr *symtab_section = NULL;
    // Ustvarimo kazalec na simbolno tabelo, ki je trenutno NULL
    Elf32_Sym *symtab = NULL;
    // Ustvarimo spremenljivko za štetje
    Elf32_Word i;
    // Ustvarimo kazalec na nizovno tabelo
    char *strtab;
    // Ustvarimo še eno spremenljivko za štetje
    int j;

    // Poiščemo simbolno tabelo
    for (i = 0; i < header->e_shnum; i++) {
        // Če je tip trenutne sekcije simbolna tabela, shranimo kazalec na to sekcijo in prekinemo zanko
        if (sections[i].sh_type == SHT_SYMTAB) {
            symtab_section = &sections[i];
            break;
        }
    }

    // Če nismo našli simbolne tabele, izpišemo napako in končamo funkcijo
    if (symtab_section == NULL) {
        printf("No symbol table found.\n");
        return;
    }

    // Pridobimo simbolno tabelo in nizovno tabelo
    symtab = (Elf32_Sym *)(elf_start + symtab_section->sh_offset);
    strtab = elf_start + sections[symtab_section->sh_link].sh_offset;

    // Za vsak simbol v simbolni tabeli
    for (i = 0; i < symtab_section->sh_size / sizeof(Elf32_Sym); i++) {
        // Za vsako spremenljivko, ki jo želimo spremeniti
        for (j = 0; j < var_count; j++) {
            // Če je ime trenutnega simbola enako imenu trenutne spremenljivke
            if (strcmp(strtab + symtab[i].st_name, variables[j]) == 0) {
                // Če simbol ni nedefiniran in je alociran
                if (symtab[i].st_shndx != SHN_UNDEF && (sections[symtab[i].st_shndx].sh_flags & SHF_ALLOC)) {
                    // Izračunamo naslov simbola v datoteki
                    unsigned int symbolAddress = sections[symtab[i].st_shndx].sh_offset + symtab[i].st_value - sections[symtab[i].st_shndx].sh_addr;
                    // Pretvorimo naslov simbola v kazalec na int
                    int *var_value = (int *)(elf_start + symbolAddress);
                    // Izpišemo trenutno vrednost spremenljivke
                    printf("  %s   0x%08x    0x%x         0x%x\n", variables[j], symtab[i].st_value, *var_value, *var_value - 1);
                    // Zmanjšamo vrednost spremenljivke za 1
                    *var_value -= 1;
                } else {
                    // Če simbol ni veljaven, izpišemo napako
                    printf("Symbol %s is not valid.\n", variables[j]);
                }
            }
        }
    }

    // Shranimo spremembe v ELF datoteko
    save_changes("hello", elf_start, header->e_shoff);
}

int main(int argc, char **argv) {
    // Ustvarimo spremenljivko za shranjevanje trenutne možnosti
    int opt;
    // Ustvarimo kazalec na pot do ELF datoteke
    char *elf_path = NULL;
    // Ustvarimo polje kazalcev na spremenljivke, ki jih želimo spremeniti
    char *variables[100];
    // Ustvarimo števec spremenljivk
    int var_count = 0;

    // Pridobimo možnosti iz ukazne vrstice
    while ((opt = getopt(argc, argv, "hlc:")) != -1) {
        // Glede na trenutno možnost izvedemo ustrezno akcijo
        switch (opt) {
        case 'h':
            // Pridobimo pot do ELF datoteke
            elf_path = argv[optind];
            // Naložimo ELF datoteko v pomnilnik
            char *elf_start = load_elf(elf_path);
            // Izpišemo glavo ELF datoteke
            elf_27255_glava(elf_start);
            break;
        case 'l':
            // Pridobimo pot do ELF datoteke
            elf_path = argv[optind];
            // Naložimo ELF datoteko v pomnilnik
            elf_start = load_elf(elf_path);
            // Izpišemo simbole ELF datoteke
            elf_27255_simboli(elf_start);
            break;
        case 'c':
            // Pridobimo pot do ELF datoteke
            elf_path = argv[optind];
            // Pridobimo seznam spremenljivk, ki jih želimo spremeniti
            char *var_list = optarg;
            // Razdelimo seznam na posamezne spremenljivke
            char *var = strtok(var_list, ",");
            while (var != NULL) {
                // Shranimo kazalec na trenutno spremenljivko in povečamo števec
                variables[var_count++] = var;
                // Pridobimo naslednjo spremenljivko
                var = strtok(NULL, ",");
            }
            // Naložimo ELF datoteko v pomnilnik
            elf_start = load_elf(elf_path);
            // Spremenimo vrednosti izbranih spremenljivk v ELF datoteki
            elf_27255_menjaj(elf_start, variables, var_count);
            break;
        default:
            printf("Usage: %s [-hlc] [OPTIONS] elf_path\n", argv[0]);
            return 1;
        }
    }
    return 0;
}
