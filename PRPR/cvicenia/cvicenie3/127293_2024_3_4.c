// Tilesova Johanna

#include <stdio.h>

int main() {
    int N;
    
    scanf("%d", &N);
    
    
    if (N < 1 || N > 15) { // overenie, či je N patri od intervalu 1 až 15
        printf("Cislo nie je z daneho intervalu\n");
        return 1; 
    }
    
    // Vypisovanie čísel v požadovanom tvare
    for (int i = 1; i <= N; i++) {
        printf("%d:", i);  // vypisanie i:
        
        // Vypisovanie čísel od (N - i + 1) po 1
        for (int j = N - i + 1; j >= 1; j--) {
            printf(" %2d", j);
        }
        
        printf("\n");
    }
    
    return 0;
}
