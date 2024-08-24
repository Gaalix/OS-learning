#include <stdio.h>

#define POZDRAV "Pozdravljen svet"

#ifdef UPORABI_KLICAJ
#define PUNCTUATION "!"
#else
#define PUNCTUATION "."
#endif

void pozdrav() {
    printf("%s%s\n", POZDRAV, PUNCTUATION);
}

int main() {
    pozdrav();
    return 0;
}