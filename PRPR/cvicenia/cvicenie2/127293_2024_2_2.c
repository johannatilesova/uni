//Tilešová Johanna

#include <stdio.h>
int main()
{
    float vyska, hmotnost;
    scanf("%f %f", &vyska, &hmotnost);
    vyska = vyska / 100;
    
    float BMI = hmotnost / (vyska*vyska);

    printf ("BMI: %.3lf\n", BMI);

    return 0;
}