// Tilesova Johanna

#include <stdio.h>
#include <stdlib.h>

#define ABECEDA 26

void inicializuj_histogram(int histogram[][ABECEDA], int riadky) {
    for (int i = 0; i < riadky; i++) {
        for (int j = 0; j < ABECEDA; j++) {
            histogram[i][j] = 0; // inicializácia histogramu na nulu
        }
    }
}

void vykresli_histogram(int histogram[][ABECEDA], int riadky) {
    // vyprintovanie abecedy
    for (char c = 'A'; c <= 'Z'; c++) {
        printf("%c ", c);
    }
    printf("\n");

    // vyprintovanie histogramu
    for (int i = 0; i < riadky; i++) {
        printf("%d ", i + 1); // vyprinotavnie čísla riadku
        for (int j = 0; j < ABECEDA; j++) {
            printf("%2d ", histogram[i][j]); // vyprintovanie počtu výskytov písmena
        }
        printf("\n");
    }
}

int main() {
    char nazov_suboru[100];
    //printf("Zadajte meno súboru: ");
    scanf("%s", nazov_suboru);

    FILE *subor = fopen(nazov_suboru, "r");
    if (subor == NULL) {
        printf("Súbor sa nepodarilo otvoriť\n");
        return 1;
    }

    int histogram[100][ABECEDA];
    int pocitadlo_riadkov = 0;

    inicializuj_histogram(histogram, 100);

    char riadok[100];
    while (fgets(riadok, sizeof(riadok), subor) != NULL && pocitadlo_riadkov < 100) {
        for (int i = 0; riadok[i] != '\0'; i++) {
            char ch = riadok[i];
            if (ch >= 'a' && ch <= 'z') {
                ch = ch - 'a' + 'A'; // premena z malych na velke pismena
            } else if (ch < 'A' || ch > 'Z') {
                continue; // ignorovanie znakov, ktoré nie sú písmená
            }
            histogram[pocitadlo_riadkov][ch - 'A']++; // inkrementácia počtu výskytov
        }
        pocitadlo_riadkov++;
    }

    fclose(subor);
    vykresli_histogram(histogram, pocitadlo_riadkov);

    return 0;
}
