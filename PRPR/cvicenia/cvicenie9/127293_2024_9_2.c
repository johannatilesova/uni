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

void hladaj_max(struct NAVSTEVNIK* navstevnici, int pocetNavstevnikov) {
    if (pocetNavstevnikov == 0) {
        printf("Zoznam navstevnikov je prazdny.\n");
        return;
    }

    int maxIndex = 0;

    for (int i = 1; i < pocetNavstevnikov; i++) {
        if (navstevnici[i].pocet > navstevnici[maxIndex].pocet) {
            maxIndex = i;
        }
    }

    printf("Najcastejsie chodi do kina: %s\n", 
           navstevnici[maxIndex].meno);
}

int main() {
    struct NAVSTEVNIK* navstevnici = NULL;
    int pocetNavstevnikov = 0;
    char meno[MAX_DLZKA_MENA];

    printf("Zadaj mena:\n");

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
    
    hladaj_max(navstevnici, pocetNavstevnikov);

    for (int i = 0; i < pocetNavstevnikov; i++) {
        free(navstevnici[i].meno);
    }
    free(navstevnici);

    return 0;
}
