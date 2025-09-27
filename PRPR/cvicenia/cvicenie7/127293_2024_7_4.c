#include <stdio.h>
#include <string.h>

#define MAX_OSOB 100
#define MAX_DLZKA_MENA 50

// Štruktúra na meno a počet návštev
struct NAVSTEVNIK {
    char meno[MAX_DLZKA_MENA];
    int pocet;
};

// Funkcia na hľadanie návštevníka v poli
int najdi_navstevnika(struct NAVSTEVNIK navstevnici[], int pocetNavstevnikov, char* meno) {
    for (int i = 0; i < pocetNavstevnikov; i++) {
        if (strcmp(navstevnici[i].meno, meno) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    struct NAVSTEVNIK navstevnici[MAX_OSOB];
    int pocetNavstevnikov = 0;
    char meno[MAX_DLZKA_MENA];

    printf("Zadajte mena:\n");

    while (1) {
        scanf("%s", meno);

        // Kontrola ukončovacieho znaku
        if (meno[0] == '#' && meno[1] == '\0') {
            break;
        }

        // Hľadanie mena v zozname
        int index = najdi_navstevnika(navstevnici, pocetNavstevnikov, meno);

        if (index == -1) {
            // Nový návštevník, pridaj ho do zoznamu
            strcpy(navstevnici[pocetNavstevnikov].meno, meno);
            navstevnici[pocetNavstevnikov].pocet = 1;
            pocetNavstevnikov++;
        } else {
            // Existujúci návštevník, zvýš počet návštev
            navstevnici[index].pocet++;
        }
    }

    // Nájsť maximálny počet návštev
    int maxPocet = 0;
    for (int i = 0; i < pocetNavstevnikov; i++) {
        if (navstevnici[i].pocet > maxPocet) {
            maxPocet = navstevnici[i].pocet;
        }
    }

    // Vypísať mená najčastejších návštevníkov
    printf("\nNajcastejsi navstevnici:\n");
    for (int i = 0; i < pocetNavstevnikov; i++) {
        if (navstevnici[i].pocet == maxPocet) {
            printf("%s\n", navstevnici[i].meno);
        }
    }

    return 0;
}
