//Tilešová Johanna

#include <stdio.h>

int main ()
{
    int polomer;
    float obvod, pi = 3.14;

    scanf("%d", &polomer);
    obvod = 2*pi*polomer;

    printf("Obvod kruhu s polomerom %d je: %.2f\n", polomer, obvod);
    
    return 0;
}
