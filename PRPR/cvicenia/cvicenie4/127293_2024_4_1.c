// Tilesova Johanna

#include <stdio.h>

// definovanie funkcie na zistenie, ci je cislo prvocislo
int zisti_prvocislo(int cislo) {    // cisla menšie alebo rovné 1 nie su prvocisla
    if (cislo <= 1) {
        return -1; 
    }
    
    for (int i = 2; i * i <= cislo; i++) {
        if (cislo % i == 0) {
            return -1; // nenájdené prvočíslo
        }
    }
    
    return 1; // naslo sa prvočíslo
}

int main() {
    int zaciatok, koniec;
    
    scanf("%d %d", &zaciatok, &koniec);
    
    if (zaciatok <= koniec) {
        
        // prechadzanie vsetkych cisel v zadanom intervale
        for (int prvocisla = zaciatok; prvocisla <= koniec; prvocisla++) {      // pridanie cisel do pola prvocisla
            if (zisti_prvocislo(prvocisla) == 1) {  
                printf("%d\n", prvocisla);
            }
        }
    } else {
        printf("Prvocislo neexistuje\n");
    }
    
    return 0;
}