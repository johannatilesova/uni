// Tilesova Johanna

#include <stdio.h>

int main() {
    int f, g;
    scanf("%d %d", &f, &g);
    
    
    // Nastavenie prvej hodnoty, ktorá je deliteľná tromi a >= f
    if (f % 3 != 0) {
        f = f + (3 - (f % 3));
    }
    
    // Výpis čísel deliteľných tromi v intervale <f, g>
    for (int i = f; i < g; i += 3) {
        printf("%d ", i);
    }
    
    printf("\n");
    
    return 0; // ukoncenie funckie
}
