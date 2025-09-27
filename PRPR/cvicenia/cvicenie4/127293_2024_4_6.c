// Tilesova Johanna

#include <stdio.h>

int main() {
    char znak; 
    int pocet_medzier = 0;

    // otvorenie suboru
    FILE *subor; 
    subor = fopen("text.txt", "r");

    // kontola ci sa subor otvoril
    if (subor == NULL) {
        printf("Subor text.txt sa nepodarilo otvorit.\n");
        return 1;
    }

    // nacitavanie znakov zo suboru
    while ((znak = getc(subor)) != EOF && znak != '*') {    // prejdenie celho suboru a po znak *
        if (znak == 'x' || znak == 'X') {   // ak precita taky znak, vypise...
            printf("Precital som X\n");
        } else if (znak == 'y' || znak == 'Y') {
            printf("Precital som Y\n");
        } else if (znak == '#' || znak == '$' || znak == '&') {
            printf("Precital som riadiaci znak\n");
        } else if (znak == ' ') {   // po precitanie medzery sa inkrementuje premenna pocet_medzier o 1
            pocet_medzier++;
        }
    }

    if (znak == '*') {  // ak sa znak rovná * vypise koniec
        printf("Koniec\n");
    }

    printf("Pocet precitanych medzier: %d\n", pocet_medzier);

    fclose(subor);
    return 0;
}
