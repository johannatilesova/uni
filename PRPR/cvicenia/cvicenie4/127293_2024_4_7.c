// Tilesova Johanna

#include <stdio.h>
#include <stdlib.h>

int main() {
    // Otvorenie súborov na čítanie
    FILE *prvy = fopen("prvy.txt", "r");
    FILE *druhy = fopen("druhy.txt", "r");

    // Kontrola, či sa súbory podarilo otvoriť
    if (prvy == NULL) {
        printf("Súbor prvy.txt sa nepodarilo otvoriť!\n");
        return 1; // Ukončenie programu s chybou
    }

    if (druhy == NULL) {
        printf("Súbor druhy.txt sa nepodarilo otvoriť!\n");
        fclose(prvy); // Zatvorenie otvoreného súboru
        return 1; // Ukončenie programu s chybou
    }

    // Počítanie rôznych znakov a dĺžok súborov
    char znak1, znak2;
    int rozne_znaky = 0;
    int dlzka_prvy = 0, dlzka_druhy = 0;

    // Porovnávanie obsahu súborov
    while (1) {
        znak1 = fgetc(prvy);
        znak2 = fgetc(druhy);

        // Zrátaj dĺžku oboch súborov
        if (znak1 != EOF) dlzka_prvy++;
        if (znak2 != EOF) dlzka_druhy++;

        // Ak sme na konci súborov
        if (znak1 == EOF && znak2 == EOF) break;

        // Ak sú znaky rôzne
        if (znak1 != znak2) {
            // Ak sú znaky platné (EOF)
            if (znak1 != EOF && znak2 != EOF) {
                rozne_znaky++; // Počítaj rozdielny znak
            }
        }
    }

    if (dlzka_prvy == dlzka_druhy && rozne_znaky == 0) {
        printf("Súbory sú identické.\n");
    } else {
        printf("Počet rôznych znakov: %d\n", rozne_znaky);
        
        if (dlzka_prvy != dlzka_druhy) {
            int rozdiel = abs(dlzka_prvy - dlzka_druhy);
            if (dlzka_prvy > dlzka_druhy) {
                printf("Jeden zo suborov je dlhsi o %d znakov.\n", rozdiel);
            } else {
                printf("Jeden zo suborov je dlhsi o %d znakov.\n", rozdiel);
            }
        }
    }

    fclose(prvy);
    fclose(druhy);

    return 0;
}
