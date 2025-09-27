// Tilesova Johanna

#include <stdio.h>
#include <math.h>

void kvadraticka_rovnica(double a, double b, double c, double *koren1, double *koren2){

    // výpočet diskriminantu
    double diskriminant = (b * b) - 4 * a * c;

    // podmienky pre počet koreňov
    if (diskriminant > 0) {
        *koren1 = (-b + sqrt(diskriminant)) / (2 * a);   
        *koren2 = (-b - sqrt(diskriminant)) / (2 * a);    

        printf("Počet koreňov kvadratickej rovnice je: 2\n");
        printf("Prvý koreň rovnice: %lf\n", *koren1);
        printf("Druhý koreň rovnice: %lf\n", *koren2);
    } else if (diskriminant == 0) {
        *koren1 = (-b) / (2 * a);    // výpočet pre jediný koreň
        *koren2 = 0;    // manuálne nastavenie druhého koreňa na 0
        printf("Počet koreňov kvadratickej rovnice je: 1\n");
        printf("Prvý koreň rovnice: %lf\n", *koren1);
        printf("Druhý koreň rovnice: %lf\n", *koren2);
    } else {
        *koren1 = *koren2 = 0;
        printf("Počet koreňov kvadratickej rovnice je: 0\n");
    }
}

int main() {

    double a, b, c, koren1, koren2;
    scanf("%lf %lf %lf", &a, &b, &c);

    // vyratanie koreňov kvadratickej rovnice
    kvadraticka_rovnica(a, b, c, &koren1, &koren2);

    return 0;
}
