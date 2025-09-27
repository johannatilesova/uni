// Tilesova Johanna

#include <stdio.h>

void vymen_ukazovatele(int *p_a, int *p_b){

    int ukazovatel = *p_a;  // deklarácia docasnej premennej
    *p_a = *p_b;    // zmena hodnoty ukazovatela p_a na hodnotu p_b
    *p_b = ukazovatel;  // hodnota ukazovatela p_b sa meni na hodnotu docasnej premennej (p_a)
}

int main() {
    int a = 10;
    int b = 14;

    printf("p_a: %p %d\n", &a, a);   // (void*)&a ziskanie ADRESY premennej a
    printf("p_b: %p %d\n", &b, b);

    // volanie funkcie na vymenu ukazovatelov
    vymen_ukazovatele(&a, &b);
    
    printf("p_a: %p %d\n", &b, a);
    printf("p_b: %p %d\n", &a, b);
    
    return 0;
}