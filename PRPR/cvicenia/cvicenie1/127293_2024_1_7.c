//Tilešová Johanna

#include <stdio.h>

int main()
{
    float cislo;
    scanf("%f", &cislo);
    printf("Cena bez dane: %.f\nPredajna cena s danou 20%%: %.1f\n", cislo, cislo+(0.2*cislo));
    return 0;
}