// Tilesova Johanna

#include <stdio.h>

int main() {
    int N;
    
    scanf("%d", &N);

    // Kontrola vstupu - N musí byť nepárne a v intervale <1, 15>
    if (N < 1 || N > 15 || N % 2 == 0) {
        printf("Zly vstup\n");
        return 1;
    }

    // Vypisovanie hviezdy veľkosti NxN
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            // Podmienky pre výpis hviezdy:
            if (i == j || i + j == N - 1 || i == N / 2 || j == N / 2) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
