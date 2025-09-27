//Tilešová Johanna

#include <stdio.h>

int main() {

    int cislo1, cislo2, vysledok;
    char znak;

    scanf("%d %d %c", &cislo1, &cislo2, &znak);

    if (znak == '+') {
        vysledok = cislo1 + cislo2;
    } else if (znak == '-') {
        vysledok = cislo1 - cislo2;
    } else if (znak == '*') {
        vysledok = cislo1 * cislo2;
    } else if (znak == '/') {

        // osetrenie podmienky pri deleni nulou
        if (cislo2 != 0) {
            vysledok = cislo1 / cislo2;
        } 
        else {
            printf("Nulou sa neda delit.\n");
            return 1; // vyhodi chybu
        }
    } else {
        printf("Zle zadana volba\n");
    }

    printf("%d %c %d = %d\n", cislo1, znak, cislo2, vysledok);

    return 0;
}