#include <stdio.h>

void vykresli_hadika(int n) {
    // Overenie nezáporného čísla a či je menšie alebo rovné 100
    if (n <= 0 || n > 100) {
        printf("Neplatný vstup!\n");
        return;  // Ukonči funkciu
    }

    char hadik[100][100];
    char zaciatok = 'a';

    // Vykreslenie hadíka do matice
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            // Každý druhý riadok vykresli od začiatku
            for (int j = 0; j < n; j++) {
                hadik[i][j] = zaciatok;
                zaciatok++;
                if (zaciatok > 'z') {  // Ak dosiahneme 'z', začneme opäť od 'a'
                    zaciatok = 'a';
                }
            }
        } else {
            // Nepárny riadok vykresli od konca
            for (int j = n - 1; j >= 0; j--) {
                hadik[i][j] = zaciatok;
                zaciatok++;
                if (zaciatok > 'z') {  // Ak dosiahneme 'z', začneme opäť od 'a'
                    zaciatok = 'a';
                }
            }
        }
    }

    // Výpis hadíka
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c ", hadik[i][j]);  // Vypíš jednotlivé znaky
        }
        printf("\n");
    }
}

int main() {
    int vstupy[2];  // Pole pre dva vstupy
    int pocet_vstupov = 2;

    // Načítanie vstupov
    for (int i = 0; i < pocet_vstupov; i++) {
        scanf("%d", &vstupy[i]);
    }

    printf("\n");
    vykresli_hadika(vstupy[0]);

    printf("\n");
    vykresli_hadika(vstupy[1]);

    return 0;
}
