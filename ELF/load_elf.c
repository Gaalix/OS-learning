#include <stdio.h>
#include <stdlib.h>
#include "load_elf.h"

char *load_elf(const char *elf_path) {
    // Odpremo datoteko za branje v binarnem načinu
    FILE *file = fopen(elf_path, "rb");
    // Preverimo, ali je bila datoteka uspešno odprta
    if (file == NULL) {
        printf("Failed to open file: %s\n", elf_path);
        return NULL;
    }

    // Premaknemo kazalec datoteke na konec datoteke
    fseek(file, 0, SEEK_END);
    // Določimo velikost datoteke
    size_t file_size = ftell(file);
    // Premaknemo kazalec datoteke nazaj na začetek datoteke
    fseek(file, 0, SEEK_SET);

    // Dodelimo blok pomnilnika, ki je dovolj velik za shranjevanje celotne ELF datoteke
    char *elf_start = malloc(file_size);
    // Preverimo, ali je bila dodelitev pomnilnika uspešna
    if (elf_start == NULL) {
        printf("Failed to allocate memory for ELF file.\n");
        fclose(file);
        return NULL;
    }

    // Preberemo celotno ELF datoteko v dodeljeni blok pomnilnika
    if (fread(elf_start, 1, file_size, file) != file_size) {
        printf("Failed to read ELF file.\n");
        free(elf_start);
        fclose(file);
        return NULL;
    }

    // Zapremo ELF datoteko
    fclose(file);
    // Vrnemo kazalec na začetek naložene ELF datoteke v pomnilniku
    return elf_start;
}