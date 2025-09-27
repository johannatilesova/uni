// retazec.c
#include <stdio.h>
#include <string.h>

void pekny_vypis_retazca(const char *retazec) {
    int dlzka = strlen(retazec);
    printf("%.*s\n", dlzka + 4, "****************************************");
    printf("* %s *\n", retazec);
    printf("%.*s\n", dlzka + 4, "****************************************");
}
