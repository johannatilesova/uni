// Tilesova Johanna

#include <stdio.h>

// Funkcia na výpočet a výpis násobkov pomocou sčítania
void nasobok(double x, int n) {
    for (int i = 1; i <= n; i++) {
        double vysledok = 0;
        for (int j = 0; j < i; j++) {
            vysledok += x;  // sčítavanie hodnoty x, i-krát
        }
        // Výpis výsledku v požadovanom formáte na 2 desatinné miesta
        printf("%.2f * %d = %.2f\n", x, i, vysledok);
    }
}

int main() {
    double x;
    int n;
    
    scanf("%lf %d", &x, &n);
    
    // Zavolanie funkcie na výpočet a výpis tabuľky násobkov
    nasobok(x, n);
    
    return 0;
}
