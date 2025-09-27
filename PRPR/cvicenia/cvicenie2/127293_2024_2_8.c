//Tilešová Johanna

#include <stdio.h>

int main() {
    int N;
    float cislo, najmensie, najvacsie;
    scanf("%d", &N);    // načítanie počtu čísel

    for (int i = 0; i < N; i++) {
        scanf("%f", &cislo);    // načítanie reálneho čísla
        if (i == 0) {
            // nastavenie prvého čísla ako najväčšie a najmenšie
            najmensie = cislo;
            najvacsie = cislo;
        } else {
            // porovnanie s najmenším a najväčším číslom
            if (cislo < najmensie) {
                najmensie = cislo;
            }
            if (cislo > najvacsie) {
                najvacsie = cislo;
            }
        }
    }
    
    printf("Minimum: %.2f\n", najmensie);    // výpis najmenšieho čísla
    printf("Maximum: %.2f\n", najvacsie);    // výpis najväčšieho čísla

    return 0;
}