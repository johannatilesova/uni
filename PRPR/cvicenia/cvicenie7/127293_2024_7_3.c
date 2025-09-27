#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pocetRetezcov;
    printf("Zadajte pocet retazcov: ");
    scanf("%d", &pocetRetezcov);
    getchar();

    char** poleRetezcov = (char**)malloc(pocetRetezcov * sizeof(char*));
    if (poleRetezcov == NULL) {
        printf("Chyba pri alokacii pamate!\n");
        return 1;
    }

    for (int i = 0; i < pocetRetezcov; i++) {
        char temp[100];
        fgets(temp, sizeof(temp), stdin);

        size_t dlzka = strlen(temp);
        if (temp[dlzka - 1] == '\n') {
            temp[dlzka - 1] = '\0';
            dlzka--;
        }


        poleRetezcov[i] = (char*)malloc((dlzka + 1) * sizeof(char));
        if (poleRetezcov[i] == NULL) {
            printf("Chyba pri alokacii pamate!\n");

            for (int j = 0; j < i; j++) {
                free(poleRetezcov[j]);
            }
            free(poleRetezcov);
            return 1;
        }
        strcpy(poleRetezcov[i], temp);
    }

    printf("\nNacitané retazce:\n");
    for (int i = 0; i < pocetRetezcov; i++) {
        printf("%s\n", poleRetezcov[i]);
    }

    for (int i = 0; i < pocetRetezcov; i++) {
        free(poleRetezcov[i]);
    }
    free(poleRetezcov);

    return 0;
}
