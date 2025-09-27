#include <stdio.h>

#define N 20

int stvorec[N][N];

// FUNKCIA NA VYPOČET MAGICKEHO ŠTVORCA
int magicky_stvorec(int n) {
    int vysledok = 0;

    // Kontrola, či je rozmer štvorca v povolenom rozsahu
    if (n > N) {
        printf("Rozmer stvorca je väčší ako povolená hodnota N.\n");
        return 0;
    } else if (n < 1) {
        printf("Rozmer stvorca musí byť aspoň 1.\n");
        return 0;
    }

    // Inicializovanie výsledku súčtom prvkov prvého riadku
    for (int j = 0; j < n; j++) {
        vysledok += stvorec[0][j];
    }

    // Skontrolovanie riadkov
    for (int i = 1; i < n; i++) {
        int sucet_riadku = 0;
        for (int j = 0; j < n; j++) {
            sucet_riadku += stvorec[i][j];
        }
        if (sucet_riadku != vysledok) {
            return 0;
        }
    }

    // Skontrolovanie stĺpcov
    for (int j = 0; j < n; j++) {
        int sucet_stlpca = 0;
        for (int i = 0; i < n; i++) {
            sucet_stlpca += stvorec[i][j];
        }
        if (sucet_stlpca != vysledok) {
            return 0;
        }
    }

    // Skontrolovanie hlavnej diagonály
    int sucet_diagonala = 0;
    for (int i = 0; i < n; i++) {
        sucet_diagonala += stvorec[i][i];
    }
    if (sucet_diagonala != vysledok) {
        return 0;
    }

    // Skontrolovanie druhej diagonály
    sucet_diagonala = 0;
    for (int i = 0; i < n; i++) {
        sucet_diagonala += stvorec[i][n - i - 1];
    }
    if (sucet_diagonala != vysledok) {
        return 0;
    }

    return 1;
}

int main() {
    int n;
    printf("Zadajte rozmer stvorca: ");
    scanf("%d", &n);

    if (n <= N) {
        printf("Zadajte prvky stvorca:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%d", &stvorec[i][j]);
            }
        }

        // Výpis výsledku
        if (magicky_stvorec(n)) {
            printf("Štvorec je magický.\n");
        } else {
            printf("Štvorec nie je magický.\n");
        }
    } else {
        printf("Rozmer stvorca je väčší ako povolená hodnota N.\n");
    }

    return 0;
}
