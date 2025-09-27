// Tilesova Johanna

#include <stdio.h>
#include <string.h>

// Funkcia na počítanie slov
int pocet_slov(char veta[]) {
    int pocet = 0;
    char *slovo = strtok(veta, " \n"); // Rozdelenie reťazca podľa medzier a nových riadkov

    while (slovo != NULL) {
        pocet++;
        slovo = strtok(NULL, " \n"); // Pokračovanie v rozdelení
    }
    return pocet;
}

// Funkcia na počítanie samohlások
int pocet_samohlasok(char veta[]) {
    int pocet_samohlasok = 0;

    for (int i = 0; i < strlen(veta); i++) {
        // Kontrola samohlások
        if (veta[i] == 'a' || veta[i] == 'e' || veta[i] == 'i' || 
            veta[i] == 'o' || veta[i] == 'u' || veta[i] == 'y' || 
            veta[i] == 'A' || veta[i] == 'E' || veta[i] == 'I' || 
            veta[i] == 'O' || veta[i] == 'U' || veta[i] == 'Y') {
            pocet_samohlasok++;
        }
    }
    return pocet_samohlasok;
}

// Funkcia na počítanie spoluhlások
int pocet_spoluhlasok(char veta[]) {
    int pocet_spoluhlasok = 0;

    for (int i = 0; i < strlen(veta); i++) {
        // Kontrola spoluhlások
        if ((veta[i] >= 'b' && veta[i] <= 'd') || (veta[i] >= 'f' && veta[i] <= 'h') || 
            (veta[i] >= 'j' && veta[i] <= 'n') || (veta[i] >= 'p' && veta[i] <= 't') || 
            (veta[i] >= 'v' && veta[i] <= 'z') ||
            (veta[i] >= 'B' && veta[i] <= 'D') || (veta[i] >= 'F' && veta[i] <= 'H') || 
            (veta[i] >= 'J' && veta[i] <= 'N') || (veta[i] >= 'P' && veta[i] <= 'T') || 
            (veta[i] >= 'V' && veta[i] <= 'Z')) {
            pocet_spoluhlasok++;
        }
    }
    
    return pocet_spoluhlasok;
}

int main() {
    char veta[256]; // Políčko pre reťazec
    char original[256]; // Políčko pre pôvodný reťazec (na počítanie samohlások a spoluhlások)

    printf("Zadajte reťazec: ");
    fgets(veta, sizeof(veta), stdin); // Načítanie reťazca zo štandardného vstupu

    // Uloženie pôvodného reťazca pre ďalšie počítania
    strcpy(original, veta);

    // Počítanie slov, samohlások a spoluhlások
    int pocet = pocet_slov(veta);
    int samohlasky = pocet_samohlasok(original);
    int spoluhlasky = pocet_spoluhlasok(original);

    // Výpis výsledkov
    printf("Pocet slov: %d\n", pocet);
    printf("Pocet samohlások: %d\n", samohlasky);
    printf("Pocet spoluhlások: %d\n", spoluhlasky);

    return 0; // Ukončenie programu
}
