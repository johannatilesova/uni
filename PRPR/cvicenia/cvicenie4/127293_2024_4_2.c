// Tilesova Johanna

#include <stdio.h>

double tyzdenna_mzda(int N) {
    double tyzdenna_mzda = 0;
    double celkova_mzda = 0;

    for (int i = 0; i < N; i++) {
        double hod_mzda, pocet_hodin, povodny_pocet_hodin;
        scanf("%lf %lf", &hod_mzda, &pocet_hodin);
        
        povodny_pocet_hodin = pocet_hodin;  // ulozenie povodneho poctu hodin
    
        if (pocet_hodin > 60) {
            pocet_hodin = 40 + 20 * 1.5 + (pocet_hodin - 60) * 2;  // 40 normalnych hodin + 20 hodin s 1.5x + extra hodiny s násobkom 2
        } else if (pocet_hodin > 40) {
            pocet_hodin = 40 + (pocet_hodin - 40) * 1.5;  // 40 normalnych hodin + extra hodin s 1.5x
        } else {
            pocet_hodin = pocet_hodin;  // normalne hodiny
        }
        
        double tyzdenna_mzda = hod_mzda * pocet_hodin;   // vypocet pre tyzdennu mzdu
        celkova_mzda += tyzdenna_mzda;  // vypocet pre celkovu mzdu
        printf("Hod. mzda: %.2lf Eur/hod, pocet hodin: %.2lf, spolu: %.2lf Eur\n", hod_mzda, povodny_pocet_hodin, tyzdenna_mzda);
    }

    return celkova_mzda;
}

int main() {
    int N;
    scanf("%d", &N);
    
    double mzda_spolu = tyzdenna_mzda(N);   // vypocet tyzdennej mzdy
    
    printf("Celková týždenná mzda: %.2lf Eur\n", mzda_spolu);

    return 0;
}
