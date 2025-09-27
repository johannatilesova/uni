// Tilesova Johanna

#include <stdio.h>

int faktorial(int n) {  // definujeme si faktorial z cisla
    int vysledok = 1;   // definujeme vysledok na hodnotu 1
    for (int i = 1; i <= n; i++) {  // for cyklus, ktory iteruje od 1 po n
        vysledok = vysledok * i;    // vypocet faktorialu násobením vysledku a i
    }
    return vysledok;    // vrati vysledok faktorialu
}

int main() {
    int N;  // deklarácia premmenej

    scanf("%d", &N);    // nacitanie premennej z klávesnice

    if (N >= 0) {   // urcenie podmienky aby N nebolo záporné cislo
        int vysledok = faktorial(N);    // vyvolanie funkcie faktorial
        printf("%d\n", vysledok);  // vypisanie vysledku pre kladné cisla
    } else {
        printf("Faktoriál pre zápozné záporné čísla neexistuje.\n"); // vypisanie pre záporné cisla
    }

    return 0; // ukoncenie programu
}
