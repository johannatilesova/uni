// Tilesova Johanna

#include <stdio.h>

// funckia na vypocet delitelov
int delitele(int x[], int pocetx, int y[], int k) {
    int pocitadlo = 0;
    for (int i = 0; i < pocetx; i++) {  // prechadzanie prvkov v poli x
        if (k % x[i] == 0) {    // podmienka, ked k je delitelne hodnotou z pola x
            y[pocitadlo] = x[i];    // pridanie hodnoty z pola x do pola y
            pocitadlo++;
        }
    }

    printf("pocety: %d\n", pocitadlo); // vypis vysledku
    printf("y: {");
    
    for (int i = 0; i < pocitadlo; i++) {   // prechadzanie prvkov v poli y
        if (i == pocitadlo - 1) {   // overenie posledneho prvku v poli y
            printf("%d", y[i]);     // vyprintovanie pola y
        } else {
            printf("%d, ", y[i]);    // vyprintovanie pola y
        }
    }
    printf("}\n");

    return pocitadlo;
}

int main() {
    int pocetx = 10;    // pocet prvkov v poli x
    int x[] = {4, 7, 10, 2, 3, 9, 6, 5, 8, 12};     // pole x
    int y[pocetx];  // pole y
    //int k = 24;

    // volanie funkcie delitele
    int pocety = delitele(x, pocetx, y, 24);

    return 0;
}