#include <stdio.h>
#include <stdlib.h>

#define LADENIE_ZAKLADNE 1
#define LADENIE_PODROBNE 2

void skontroluj_sumu(const char* filename, int ladiaci_mod) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Otvorenie suboru\n");
        printf("Nepodarilo sa otvorit subor\n");
        exit(1);
    }

    if (ladiaci_mod == LADENIE_ZAKLADNE || ladiaci_mod == LADENIE_PODROBNE) {
        printf("Otvorenie suboru\n");
    }
    if (ladiaci_mod == LADENIE_PODROBNE) {
        printf("Subor otvoreny\n");
    }

    double ocakavany_sucet;
    if (fscanf(file, "%lf", &ocakavany_sucet) != 1) {
        printf("Nepodarilo sa nacitat prvu hodnotu\n");
        fclose(file);
        exit(1);
    }

    if (ladiaci_mod == LADENIE_ZAKLADNE || ladiaci_mod == LADENIE_PODROBNE) {
        printf("Kontrola sumy\n");
    }

    double sucet = 0.0;
    double hodnota;
    while (fscanf(file, "%lf", &hodnota) == 1) {
        sucet += hodnota;
        if (ladiaci_mod == LADENIE_PODROBNE) {
            printf("Sucet: %g\n", sucet);
        }
    }

    if (sucet == ocakavany_sucet) {
        printf("Suma je spravna\n");
    } else {
        printf("Suma je nespravna\n");
    }

    if (ladiaci_mod == LADENIE_PODROBNE) {
        printf("Suma skontrolovana\n");
    }

    if (fclose(file) != 0) {
        printf("Nepodarilo sa zatvorit subor\n");
        exit(1);
    }

    if (ladiaci_mod == LADENIE_ZAKLADNE || ladiaci_mod == LADENIE_PODROBNE) {
        printf("Zatvaranie suboru\n");
    }
    if (ladiaci_mod == LADENIE_PODROBNE) {
        printf("Subor zatvoreny\n");
    }
}

int main() {
    skontroluj_sumu("suma.txt", LADENIE_ZAKLADNE);
    skontroluj_sumu("suma.txt", LADENIE_PODROBNE);
    return 0;
}