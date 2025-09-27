//Tilesova Johanna 127293

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DLZKA 100

typedef enum {
    CENA_KUS,
    CENA_KILOGRAM
} TypCeny;

typedef struct {
    TypCeny typ;
    union {
        double cena_za_kus;
        double cena_za_kilogram;
    };
} Cena;

typedef struct ZoznamNakupov {
    char nazov[MAX_DLZKA];
    char vyrobca[MAX_DLZKA];
    Cena cena;
    double mnozstvo;
    struct ZoznamNakupov* dalsi;
} ZoznamNakupov;

void vypis_zoznam(ZoznamNakupov* prvy) {
    if (prvy == NULL) {
        printf("Zoznam je prazdny\n");
        return;
    }

    ZoznamNakupov* aktualny = prvy;

    printf("--------------------\n");
    while (aktualny != NULL) {
        printf("Nazov: %s\n", aktualny->nazov);
        printf("Vyrobca: %s\n", aktualny->vyrobca);

        // Výpis ceny a množstva podľa typu
        if (aktualny->cena.typ == CENA_KUS) {
            printf("Cena za kus: %.2lf\n", aktualny->cena.cena_za_kus);
            printf("Pocet kusov: %.0lf\n", aktualny->mnozstvo);
        } else if (aktualny->cena.typ == CENA_KILOGRAM) {
            printf("Cena za kilogram: %.2lf\n", aktualny->cena.cena_za_kilogram);
            printf("Hmotnost (kg): %.2lf\n", aktualny->mnozstvo);
        }

        printf("--------------------\n");
        aktualny = aktualny->dalsi;
    }
}

void uvolni_zoznam(ZoznamNakupov* prvy) {
    ZoznamNakupov* aktualny = prvy;
    while (aktualny != NULL) {
        ZoznamNakupov* dalsi = aktualny->dalsi;
        free(aktualny);
        aktualny = dalsi;
    }
}

void pridaj_novy_tovar(ZoznamNakupov** prvy) {
    char nazov[MAX_DLZKA];
    char vyrobca[MAX_DLZKA];
    char jednotka[10];
    double cena;
    double mnozstvo;

    printf("Zadajte nazov tovaru: ");
    scanf(" %99s", nazov);

    printf("Zadajte vyrobcu tovaru: ");
    scanf(" %99s", vyrobca);

    printf("Zadajte jednotku (kus alebo kg): ");
    scanf(" %9s", jednotka);

    // Zistenie typu ceny na základe zadanej jednotky
    TypCeny typ_ceny;
    if (strcmp(jednotka, "kus") == 0) {
        typ_ceny = CENA_KUS;
    } else if (strcmp(jednotka, "kg") == 0) {
        typ_ceny = CENA_KILOGRAM;
    } else {
        printf("Neplatna jednotka.\n");
        return;
    }

    printf("Zadajte cenu: ");
    if (scanf("%lf", &cena) != 1 || cena < 0) {
        printf("Neplatna cena.\n");
        return;
    }

    // Vypýtanie množstva podľa jednotky
    if (typ_ceny == CENA_KUS) {
        printf("Zadajte pocet kusov (cele cislo): ");
        if (scanf("%lf", &mnozstvo) != 1 || mnozstvo <= 0 || (int)mnozstvo != mnozstvo) {
            printf("Neplatny pocet kusov. Skuste znova.\n");
            return;
        }
    } else if (typ_ceny == CENA_KILOGRAM) {
        printf("Zadajte hmotnost v kilogramoch: ");
        if (scanf("%lf", &mnozstvo) != 1 || mnozstvo <= 0) {
            printf("Neplatna hmotnost. Skuste znova.\n");
            return;
        }
    }

    ZoznamNakupov* novy = (ZoznamNakupov*)malloc(sizeof(ZoznamNakupov));
    if (novy == NULL) {
        printf("Chyba: Nepodarilo sa alokovat pamat\n");
        return;
    }

    // Naplnenie údajov
    strcpy(novy->nazov, nazov);
    strcpy(novy->vyrobca, vyrobca);
    novy->mnozstvo = mnozstvo;

    // Nastavenie ceny a typu
    novy->cena.typ = typ_ceny;
    if (typ_ceny == CENA_KUS) {
        novy->cena.cena_za_kus = cena;
    } else {
        novy->cena.cena_za_kilogram = cena;
    }
    novy->dalsi = NULL;

    // Vloženie do zoznamu na správne miesto podľa názvu
    if (*prvy == NULL || strcmp((*prvy)->nazov, nazov) > 0) {
        novy->dalsi = *prvy;
        *prvy = novy;
    } else {
        ZoznamNakupov* aktualny = *prvy;
        while (aktualny->dalsi != NULL && strcmp(aktualny->dalsi->nazov, nazov) < 0) {
            aktualny = aktualny->dalsi;
        }
        novy->dalsi = aktualny->dalsi;
        aktualny->dalsi = novy;
    }

    printf("Tovar bol pridany.\n");
}

void zmaz_tovar(ZoznamNakupov** prvy) {
    if (*prvy == NULL) {
        printf("Zoznam je prazdny\n");
        return;
    }

    char nazov[MAX_DLZKA];
    printf("Zadajte nazov tovaru na vymazanie: ");
    scanf(" %99s", nazov);

    ZoznamNakupov* aktualny = *prvy;
    ZoznamNakupov* predchadzajuci = NULL;

    while (aktualny != NULL) {
        if (strcmp(aktualny->nazov, nazov) == 0) {
            if (predchadzajuci == NULL) {
                *prvy = aktualny->dalsi;
            } else {
                predchadzajuci->dalsi = aktualny->dalsi;
            }
            free(aktualny);
            printf("Tovar bol vymazany.\n");
            return;
        }
        predchadzajuci = aktualny;
        aktualny = aktualny->dalsi;
    }

    printf("Tovar s nazvom '%s' nebol najdeny.\n", nazov);
}

int main() {
    ZoznamNakupov* prvy = NULL;
    char volba;

    printf("Moznosti:\n'p' - Pridat tovar\n'z' - Zmazat tovar\n'v' - Vypisat zoznam\n'k' - Koniec\n");

    while (1) {
        printf("Zadajte volbu: ");
        scanf(" %c", &volba);

        switch (volba) {
            case 'p':
                pridaj_novy_tovar(&prvy);
                break;
            case 'z':
                zmaz_tovar(&prvy);
                break;
            case 'v':
                vypis_zoznam(prvy);
                break;
            case 'k':
                uvolni_zoznam(prvy);
            default:
                printf("Neplatna volba.\n");
                break;
        }
    }

    return 0;
}
