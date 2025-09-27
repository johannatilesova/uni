#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DLZKA_MENA 50

struct NAVSTEVNIK {
    char* meno;
    int pocet;
};

int najdi_navstevnika(struct NAVSTEVNIK* navstevnici, int pocetNavstevnikov, char* meno) {
    for (int i = 0; i < pocetNavstevnikov; i++) {
        if (strcmp(navstevnici[i].meno, meno) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    struct NAVSTEVNIK* navstevnici = NULL;
    int pocetNavstevnikov = 0;
    char meno[MAX_DLZKA_MENA];

    printf("Zadajte mena:\n");

    while (1) {
        scanf("%s", meno);

        if (meno[0] == '#' && meno[1] == '\0') {
            break;
        }

        int index = najdi_navstevnika(navstevnici, pocetNavstevnikov, meno);

        if (index == -1) {
            navstevnici = realloc(navstevnici, (pocetNavstevnikov + 1) * sizeof(struct NAVSTEVNIK));
            if (!navstevnici) {
                fprintf(stderr, "Nepodarilo sa alokovať pamäť.\n");
                return 1;
            }

            navstevnici[pocetNavstevnikov].meno = malloc(strlen(meno) + 1);
            if (!navstevnici[pocetNavstevnikov].meno) {
                fprintf(stderr, "Nepodarilo sa alokovať pamäť.\n");
                return 1;
            }
            strcpy(navstevnici[pocetNavstevnikov].meno, meno);
            navstevnici[pocetNavstevnikov].pocet = 1;
            pocetNavstevnikov++;
        } else {
            navstevnici[index].pocet++;
        }
    }

    int maxPocet = 0;
    for (int i = 0; i < pocetNavstevnikov; i++) {
        if (navstevnici[i].pocet > maxPocet) {
            maxPocet = navstevnici[i].pocet;
        }
    }

    printf("\nNajcastejsi navstevnici:\n");
    for (int i = 0; i < pocetNavstevnikov; i++) {
        if (navstevnici[i].pocet == maxPocet) {
            printf("%s\n", navstevnici[i].meno);
        }
    }

    for (int i = 0; i < pocetNavstevnikov; i++) {
        free(navstevnici[i].meno);
    }
    free(navstevnici);

    return 0;
}
