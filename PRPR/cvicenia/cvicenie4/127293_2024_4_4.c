// Tilesova Johanna

#include <stdio.h>

int main() {
    float x;
    FILE *subor;

    
    printf("Zadajte reálne číslo x: ");
    scanf("%f", &x);

    // otvorenie súboru na zápis
    subor = fopen("nasobky.txt", "w");
    
    if (subor == NULL) {    // kontola ci sa subor otvoril 
        printf("Chyba pri otváraní súboru.\n"); 
        return 1; 
    }

    // funkcia pre vyratanie násobkov čísla x a nasledny zápis do súboru
    for (int i = 1; i < 10; i++) {
        float nasobok = i * x;
        fprintf(subor, "%2d * %.2f = %.2f\n", i, x, nasobok);
    }

    // uzatvorenie súboru
    fclose(subor);

    // printf("Násobky boli zapísané do súboru 'nasobky.txt'.\n");

    return 0;
}
