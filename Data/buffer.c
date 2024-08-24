#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

// Funkcija za rezervacijo dinamičnega pomnilnika dane velikosti
uint16_t *buffer_alloc(size_t size) {
    // Dodeli pomnilnik za buffer velikosti 'size' in vrni kazalec na začetek
    return (uint16_t *) malloc(size * sizeof(uint16_t));
}

// Funkcija za sprostitev dinamičnega pomnilnika na danem naslovu
void buffer_free(uint16_t *buffer) {
    free(buffer);
}

// Funkcija za zapisovanje podatkov v pomnilnik na danem naslovu v določenem vrstnem redu
void buffer_fill(uint16_t *buffer) {
    ((char *)buffer)[0] = 'N'; // char (1 bajt)
    ((char *)buffer)[1] = 'P'; // char (1 bajt)
    ((char *)buffer)[2] = 'O'; // char (1 bajt)
    ((float *)&(((char *)buffer)[3]))[0] = 3.141593; // float (4 bajti)
    ((uint32_t *)&(((char *)buffer)[7]))[0] = 27259; // uint32_t (4 bajti)
    ((char *)buffer)[11] = '!'; // char (1 bajt)
}

// Funkcija za izpisovanje vsebine pomnilnika na danem naslovu
void buffer_print(uint16_t *buffer) {
    printf("%c\n", ((char *)buffer)[0]); // char
    printf("%c\n", ((char *)buffer)[1]); // char
    printf("%c\n", ((char *)buffer)[2]); // char
    printf("%f\n", *((float *)(((char *)buffer) + 3))); // float
    printf("%u\n", *((uint32_t *)(((char *)buffer) + 7))); // uint32_t
    printf("%c\n", ((char *)buffer)[11]); // char
}