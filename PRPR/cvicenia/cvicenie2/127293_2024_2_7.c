//Tilešová Johanna

#include <stdio.h>

int main() {
    int a, b, c, d, e;
    scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);    // načítanie piatich premenných

    double vystup1;    // double = des. číslo s väčšou presnosťou ako float
    int vystup2;

    // Prvý výraz: e / --a * b++ / c++
    vystup1 = (double) e / --a * b++ / c++; 
    // --a zníži hodnotu a o 1
    // b++ použije pôvodnú hodnotu b a až potom ju zvýši
    // c++ použije pôvodnú hodnotu c a až potom ju zvýši

    //a %= b = d + 1 + e / 2;
    vystup2 = a %= b = d + 1 + e / 2;;

    printf("%d \n", (int)vystup1); // dve des. miesta za čiarkou pre prvý výsledok
    printf("%d \n", vystup2);   // výstup ako celé číslo pre druhý výsledok

    return 0;
}
