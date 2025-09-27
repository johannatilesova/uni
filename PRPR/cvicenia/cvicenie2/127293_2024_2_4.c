//Tilešová Johanna

#include <stdio.h>
int main()
{
    char pismeno1, pismeno2;
    scanf("%c %c", &pismeno1, &pismeno2);

    char velke_pismeno1 = pismeno1 - 32;
    char velke_pismeno2 = pismeno2 - 32;

    int hodnota1, hodnota2;
    hodnota1 = (int)velke_pismeno1;
    hodnota2 = (int)velke_pismeno2;
    
    printf("%c %d\n", velke_pismeno1, hodnota1);
    printf("%c %d\n", velke_pismeno2, hodnota2);

    return 0;
}