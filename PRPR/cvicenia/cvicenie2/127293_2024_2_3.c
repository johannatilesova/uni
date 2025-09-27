//Tilešová Johanna

#include <stdio.h>

int main() {
    double cislo;
    int cele_cislo, zaokruhlene_cislo;
   
    scanf("%lf", &cislo);   //nacitanie zadaneho cisla
    
    cele_cislo = (int)cislo;    //premena realneho cisla na cele cislo (z double na int)

    if (cislo - cele_cislo >= 0.5) { 
        //zaokruhlenie cisla, ak je cislo za des. ciarkou vacsie ako 0.5
        zaokruhlene_cislo = cele_cislo + 1;
    } else {
        zaokruhlene_cislo = cele_cislo;
    }

    printf("%d %d\n", cele_cislo, zaokruhlene_cislo); //vypisanie vysledku

    return 0;
}

