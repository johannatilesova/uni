// Tilešová Johanna

#include <stdio.h>

void maximum (int a, int b, int *max) {
    if (a > b) {
        *max = a;
    } else {
        *max = b;
    }
}

int main (){
    int a, b, max;
    scanf("%d %d", &a, &b);
    maximum(a, b, &max);
    printf("Cislo %d je vacsie.\n", max);
    return 0;
}