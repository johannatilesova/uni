// Tilesova Johanna

#include <stdio.h>

int main() {
    
    FILE *subor1, *subor2;
    subor1 = fopen("znak.txt", "r");     // otvorenie suboru znak.txt na citanie
    subor2 = fopen("novy.txt", "w");     // otvorenie suboru novy.txt na zapisovanie

    if (subor1 == NULL) {    // kontola ci sa subor1 otvoril 
        printf("Subor znak.txt sa nepodarilo otvorit.\n");
        return 1;
    }

    if (subor2 == NULL) {   // kontola ci sa subor2 otvoril 
        printf("Subor novy.txt sa nepodarilo vytvorit.\n");
        return 1;
    }

    char znak; 
    scanf(" %c", &znak);  

// prepisanie textu zo suboru1 do suboru2
    if (znak == 's') {  // ak znak je rovny s
        while ((znak = getc(subor1)) != EOF) {  // vezme sa text zo suboru1
            putc(znak, subor2); // prepise sa do suboru2
        }
        //printf("Znaky boli zapisane do suboru novy.txt.\n");
    } else {
        //printf("Obsah zo suboru znak.txt:\n");
        rewind(subor1); // opakovane precita subor1
        while ((znak = getc(subor1)) != EOF) {  // vypisanie obsahu suboru1 na obrazovku 
            printf("%c", znak);
        }
        printf("\n");
    }
}