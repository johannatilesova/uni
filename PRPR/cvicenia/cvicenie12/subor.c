// subor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pekny_vypis_suboru(const char *cesta_k_suboru, int sirka_ramka) {
    FILE *subor = fopen(cesta_k_suboru, "r");
    if (!subor) {
        printf("Súbor sa nenašiel.\n");
        return;
    }

    char riadok[1024];
    char slovo[100];
    int riadok_dlzka = 0;

    // Orámovanie
    for (int i = 0; i < sirka_ramka + 4; i++) printf("*");
    printf("\n");

    while (fscanf(subor, "%s", slovo) != EOF) {
        int slovo_dlzka = strlen(slovo);
        if (riadok_dlzka + slovo_dlzka + 1 > sirka_ramka) {
            printf("* %s%*s *\n", riadok, sirka_ramka - riadok_dlzka, "");
            strcpy(riadok, "");  // Vyčisti riadok
            riadok_dlzka = 0;
        }
        if (riadok_dlzka > 0) {
            strcat(riadok, " ");
            riadok_dlzka++;
        }
        strcat(riadok, slovo);
        riadok_dlzka += slovo_dlzka;
    }

    if (riadok_dlzka > 0) {
        printf("* %s%*s *\n", riadok, sirka_ramka - riadok_dlzka, "");
    }

    // Ukončovacie orámovanie
    for (int i = 0; i < sirka_ramka + 4; i++) printf("*");
    printf("\n");

    fclose(subor);
}
