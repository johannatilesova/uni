//Tilešová Johanna

#include <stdio.h>

int main() {

    int cislo1, cislo2, vysledok;
    char znak;

    scanf("%d %d %c", &cislo1, &cislo2, &znak);

    switch (znak) {
    case '+':
        vysledok = cislo1 + cislo2;
        break;
    case '-':
        vysledok = cislo1 - cislo2;
        break;
    case '*':
        vysledok = cislo1 * cislo2;
        break;
    case '/':
        if (cislo2 != 0) {
            vysledok = cislo1 / cislo2;
        } 
        else {
            printf("Nulou sa neda delit.\n");
            return 1;
        }
        break;
    default:
        printf("Zle zadana volba\n");
        break;
    }

    printf("%d %c %d = %d\n", cislo1, znak, cislo2, vysledok);
    
    return 0;
}