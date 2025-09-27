// Tilesova Johanna

#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    
    // Načítanie počtu znakov
    //printf("Zadajte počet znakov: ");
    scanf("%d", &n);
    
    // Alokovanie pamäte pre n znakov
    char *znaky = (char *)malloc(n * sizeof(char));
    if (znaky == NULL) {
        printf("Chyba pri alokácii pamäte!\n");
        return 1; // Ukončenie programu v prípade chyby
    }
    
    // Načítanie znakov zo vstupu
    for (int i = 0; i < n; i++) {
        scanf(" %c", &znaky[i]);
    }
    
    // Výpis znakov odzadu
    for (int i = n - 1; i >= 0; i--) {
        printf("%c", *(znaky + i)); // Ukazovateľová aritmetika
    }
    printf("\n");
    
    // Uvoľnenie alokovanej pamäte
    free(znaky);
    
    return 0;
}
