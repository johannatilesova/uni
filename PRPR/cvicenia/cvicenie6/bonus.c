#include <stdio.h>

char zMalychNaVelke(char pismeno) {
    if (pismeno >= 'a' && pismeno <= 'z') {
        pismeno = pismeno - ('a' - 'A');
    }
    return pismeno;
}

void Vykreslenie_S4(int n, char start_pozicia, char velkePismeno) {
    int width = 2 * n - 1;

    start_pozicia = zMalychNaVelke(start_pozicia);

    if (velkePismeno == 'Z') {
        if (n == 0 || n < 0) {
            printf("nie je mozny vypis\n");
        }
        printf("");
        return;
    }

    if (velkePismeno == 'V') {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < width; j++) {
                int min = i < j ? i : j;
                min = min < width - i - 1 ? min : width - i - 1;
                min = min < width - j - 1 ? min : width - j - 1;

                char ch = start_pozicia + min;

                if (ch > 'Z') {
                    ch = 'A' + (ch - 'Z' - 1);
                }

                printf("%c", ch);
            }
            printf("\n");
        }
    }
}

int main() {
    int n;
    char start_pozicia, velkePismeno;

    scanf("%d, %c, %c", &n, &start_pozicia, &velkePismeno);

    Vykreslenie_S4(n, start_pozicia, velkePismeno);

    return 0;
}
