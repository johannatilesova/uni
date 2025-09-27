//Tilešová Johanna

#include <stdio.h>

int main() {
    int i, pole[11];
    int spravnaPostupnost = 1;

    // nacitanie 11 celych cisel
    for ( i = 0; i < 11; i++) {
        scanf("%d", &pole[i]);
    }

    // kontrola prvého cisla, ci je z rozsahu 0 az 10
    if (pole[0] < 0 || pole[0] > 10) {
        spravnaPostupnost = 0;
    }

    // kontrola kazdeho dalsieho cisla
    for (i = 2; i < 11; i++) {
        if (pole[i] > 2 * pole[i-1] || pole[i] < pole[i-1] / 2) {
            spravnaPostupnost = 0;
            break;
        }
    }

    if (spravnaPostupnost) {
        printf("Postupnost je spravna\n");
    } else {
        printf("Postupnost nie je spravna\n");
    }

    return 0;
}