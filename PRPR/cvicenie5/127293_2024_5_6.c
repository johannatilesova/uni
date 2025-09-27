#include <stdio.h>
#include <stdlib.h>

// funkcia na dynamickú alokáciu 1D poľa a načítanie hodnôt z klávesnice
void alokuj_1D_pole(int **m, int velkost) {
    *m = (int *)malloc(velkost * sizeof(int));  // alokácia pamäte pre pole
    for (int i = 0; i < velkost; i++) { // načítanie hodnôt z klávesnice do poľa
        scanf("%d", &(*m)[i]);
    }
}


// funkcia na výpis obsahu 1D poľa do konzoly
void vypis_1D_pole(int *m, int velkost) {
    for (int i = 0; i < velkost; i++) {
        if (i == velkost - 1) {
            printf("%d", m[i]);     // výpis hodnôt do poľa
        } else {
            printf("%d,", m[i]);
        }
    }
    printf("\n");
}


// funkcia na nájdenie pozície maximálnej hodnoty v poli
int maximum_poz(int *m, int velkost) {
    int max = m[0];
    int max_poz = 0;
    for (int i = 1; i < velkost; i++) {
        if (m[i] > max) {
            max = m[i];     // nájdenie maximálnej hodnoty v poli
            max_poz = i;    // uloženie pozície maximálnej hodnoty
        }
    }
    return max_poz;     // vrátenie pozície maximálnej hodnoty
}


// funkcia na nájdenie pozície minimálnej hodnoty v poli
int minimum_poz(int *m, int velkost) {
    int min = m[0];
    int min_poz = 0;
    for (int i = 1; i < velkost; i++) {
        if (m[i] < min) {
            min = m[i];     // nájdenie minimalnej hodnoty v poli
            min_poz = i;    // uloženie pozície minimalnej hodnoty
        }
    }
    return min_poz;     // vrátenie pozície minimalnej hodnoty
}


int main() {
    int velkost;    // deklarácia premennej
    scanf("%d", &velkost);  // nacitanie velkosti velkosti pola

    int *pole;
    alokuj_1D_pole(&pole, velkost); // volanie funkcie na alokáciu a načítanie hodnôt do poľa


    printf("Vypis pola: ");
    vypis_1D_pole(pole, velkost);   // volanie funkcie na výpis hodnôt poľa

    int max_pozicia = maximum_poz(pole, velkost);   // získanie pozície maximálnej hodnoty v poli
    int min_pozicia = minimum_poz(pole, velkost);   // získanie pozície minimálnej hodnoty v poli

    printf("Maximalna hodnota je %d a nachadza sa na pozicii %d\n", pole[max_pozicia], max_pozicia);
    printf("Minimalna hodnota je %d a nachadza sa na pozicii %d\n", pole[min_pozicia], min_pozicia);

    free(pole);

    return 0;
}
