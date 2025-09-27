// Tilesova Johanna

#include <stdio.h>

int main() {
    FILE *subor;
    subor = fopen("veta.txt", "r");
    if (subor == NULL) {
        printf("Súbor sa nepodarilo otvoriť!\n");
        return 1;
    }

    // Znaky samohlások, súhlások a číslic
    char samohlasky[] = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
    char spoluhlasky[] = {
        'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 
        't', 'v', 'w', 'x', 'z', 'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 
        'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Z'
    };  
    char cislice[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; 

    char veta;
    int pocet_samohlasok = 0, pocet_spoluhlasok = 0, pocet_cislic = 0; // počítadlá

    // Prechádzanie znakov v súbore
    while ((veta = fgetc(subor)) != EOF) {

        for (int i = 0; i < sizeof(samohlasky); i++) {
            if (veta == samohlasky[i]) {
                pocet_samohlasok++;
                break;
            }
        }
        
        for (int i = 0; i < sizeof(spoluhlasky); i++) {
            if (veta == spoluhlasky[i]) {
                pocet_spoluhlasok++;
                break;
            }
        }
        
        for (int i = 0; i < sizeof(cislice); i++) {
            if (veta == cislice[i]) {
                pocet_cislic++;
                break;
            }
        }
    }

    printf("Pocet samohlasok: %d, pocet hlasok: %d, pocet cislic: %d\n", pocet_samohlasok, pocet_spoluhlasok, pocet_cislic);

    fclose(subor);

    return 0;
}
