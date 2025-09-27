//Tilešová Johanna

#include <stdio.h>

int main()
{
    float x, y, z;
    scanf("%g %g %g", &x, &y, &z);
    
    float priemer = (x + y + z) / 3;
    printf("Priemer cisel %g %g %g je %.3f\n", x, y, z, priemer);

    return 0;
}
