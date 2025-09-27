// Tilesova Johanna

#include <stdio.h>

int main() {
    FILE *file;
    double cislo, sucet = 0;
    
    // Otvorenie súboru cisla.txt na čítanie
    file = fopen("cisla.txt", "r");
    
    // Kontrola, či sa súbor otvoril správne
    if (file == NULL) {
        printf("Neexistuje subor: cisla.txt.\n");
        return 1;  // Ukončenie programu, ak sa súbor nedá otvoriť
    }
    
    // Načítanie a výpis reálnych čísel zo súboru
    while (fscanf(file, "%lf", &cislo) != EOF) {
        printf("%.2f\n", cislo);  // Výpis čísla na dve desatinné miesta
        sucet += cislo;           // Sčítanie načítaných čísel
    }
    
    // Výpis výsledného súčtu
    printf("Sucet cisel: %.2f\n", sucet);
    
    // Zatvorenie súboru
    fclose(file);
    
    return 0;
}
