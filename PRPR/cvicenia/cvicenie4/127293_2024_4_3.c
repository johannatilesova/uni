// Tilesova Johanna

#include <stdio.h>

// Definicia funkcie na zistenie reverzného čísla
long reverzne_cislo(long x) {
    long reverzne_cislo = 0, zvysok;
    while (x != 0) {
        zvysok = x % 10;
        reverzne_cislo = reverzne_cislo * 10 + zvysok;
        x = x / 10;
    }
    return reverzne_cislo;
}

// Funkcia na zistenie, či je číslo palindróm
int palindrom(long x) {
    long povodne_cislo = x;
    long reverzne = reverzne_cislo(x);
    if (povodne_cislo == reverzne) {
        return 1; // Ak je číslo palindróm, vráti 1
    } else {
        return 0; // Ak nie je číslo palindróm, vráti 0
    }
}

int main() {
    long cislo;
    scanf("%ld", &cislo);

    while (cislo != 0) {
        long revrzne = reverzne_cislo(cislo);
        printf("%ld\n", revrzne);

        if (palindrom(cislo)) {
            printf("Cislo %ld je palindrom.\n", cislo);
        } else {
            printf("Cislo %ld nie je palindrom.\n", cislo);
        }

        scanf("%ld", &cislo);
    }

    return 0;
}