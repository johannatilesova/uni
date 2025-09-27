// Tilešová Johanna

#include <stdio.h>

// Funkcia na načítanie dvoch reálnych čísel
void nacitaj(double *a, double *b) {
    scanf("%lf %lf", a, b);
}

// Funkcia na výpočet obsahu a obvodu obdĺžnika
void vypocitaj(double a, double b, double *obsah, double *obvod) {
    *obsah = a * b;
    *obvod = 2 * (a + b);
}

int main() {
    double a, b;
    double obsah, obvod;
    
    nacitaj(&a, &b);
    
    vypocitaj(a, b, &obsah, &obvod);
    
    printf("Obsah: %.3lf\n", obsah);
    printf("Obvod: %.3lf\n", obvod);
    
    return 0;
}