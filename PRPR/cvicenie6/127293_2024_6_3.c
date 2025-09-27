// Tilesova Johanna

#include <stdio.h>
#include <stdlib.h> // kniznica pre malloc a free

// funkcia na dynamickú alokáciu 2D poľa
int **alokuj_2D_pole(int n, int m) {
    
    // alokuje sa priestor pre pole ukazovatelov int
    int **pole = (int **)malloc(n * sizeof(int *)); // alokuje sa n riadkov
    
    // pre kazdy riadok v poli sa alokuje priestor pre m integerov
    for (int i = 0; i < n; i++) {
        pole[i] = (int *)malloc(m * sizeof(int));
    }
    
    // vracia adresu novej alokovanej pamäte
    return pole;
}

// funkcia na výpis 2D pola
void vypis_2D_pole(int **pole, int n, int m) {
    
    // prechadzanie kazdeho riadku a stlpca pola a vypisuje hodnoty
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", pole[i][j]);
        }
        printf("\n");
    }
}

// funkcia na uvolnenie pamate
void uvolni(int **pole, int n) {
    
    // prechadzanie kazdeho riadku pola a uvolnuje pamat
    for (int i = 0; i < n; i++) {
        free(pole[i]);
    }
    
    free(pole);
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m); // nacitanie rozmerov pola z klavesnice

    // alokuje 2D pole
    int **pole = alokuj_2D_pole(n, m);

    // nacitava hodnoty do pola zo vstupu
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &pole[i][j]);
        }
    }

    // vypisanie 2D pole
    printf("Vypis pola:\n");
    vypis_2D_pole(pole, n, m);

    // uvolnenie alokovanej pamate
    uvolni(pole, n);

    return 0;
}
