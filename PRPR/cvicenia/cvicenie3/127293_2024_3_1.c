// Tilesova Johanna

#include <stdio.h>

int main() {
    int N;
    int pocet = 0;
    
    scanf("%d", &N); // načítanie celého čísla N zo vstupu

    for (int i = 0; i < N; i++) {   // načítanie N riadkov s celými číslami
        int cislo;
        scanf("%d", &cislo);
        if (cislo > 0 && cislo < 100) { //kontola, ci cisla patria do intervalu (0,100)
            pocet++;    // ak ano, zvysi sa pocet
        }
    }

    printf("%d\n", pocet);  // vypisanie poctu cisel, ktore splnaju podmienku
    
    return 0;
}
