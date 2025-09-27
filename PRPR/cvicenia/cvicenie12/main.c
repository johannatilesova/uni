// main.c
#include <stdio.h>
#include "retazec.c"
#include "subor.c"

int main() {
    char nazov_suboru[100];
    int sirka_ramka;

    printf("Zadajte názov súboru: ");
    scanf("%s", nazov_suboru);

    printf("Zadajte šírku rámika: ");
    scanf("%d", &sirka_ramka);

    // Pekný výpis názvu súboru
    pekny_vypis_retazca(nazov_suboru);

    // Pekný výpis obsahu súboru
    pekny_vypis_suboru(nazov_suboru, sirka_ramka);

    return 0;
}
