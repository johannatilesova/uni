#include <stdio.h>
#include <stdlib.h>

unsigned int invert(unsigned int x, int i, int n) {
    // Kontrola platnosti vstupov
    if (i < 0 || n < 0 || (i + n) > (int)(sizeof(unsigned int) * 8)) {
        printf("Neplatna hodnota i alebo n\n");
        exit(1);
    }
    // Vytvorenie masky a posunutie na správnu pozíciu
    unsigned int mask = ((1U << n) - 1) << i;
    // Invertovanie bitov pomocou XOR operácie
    return x ^ mask;
}

int main() {
    unsigned int x;
    int i, n;

    printf("Zadajte hodnoty pre x, i a n: ");
    // Čítanie vstupných hodnôt
    if (scanf("%u %d %d", &x, &i, &n) != 3) {
        printf("Neplatny vstup\n");
        return 1;
    }

    // Kontrola platnosti vstupných hodnôt pre i a n
    if (i < 0 || n < 0 || (i + n) > (int)(sizeof(unsigned int) * 8)) {
        printf("Neplatna hodnota i alebo n\n");
        return 1;
    }

    // Volanie funkcie invert a výpis výsledku
    unsigned int result = invert(x, i, n);
    printf("Inverzia: %u\n", result);

    return 0;
}