// Tilesova Johanna

#include <stdio.h>
#include <math.h>

void mocnina(double x, int n) { // funckia mocniny
    for (int i = 1; i <= n; i++) {  // prechádzanie mocniny od 1 po n
        double vysledok = pow(x, i);    //vypocitanie i-tej mocniny cisla x
        printf("%.2lf^%d = %.2lf\n", x, i, vysledok);   // printnutie vysledku
    }
}

int main() {
    double x;   // deklaracia premennych
    int n;

    scanf("%lf %d", &x, &n);    // načítanie vstupnych hodnot

    mocnina(x, n);  // zavolanie funkcie mocniny s načítanými hodnotami

    return 0;
}
