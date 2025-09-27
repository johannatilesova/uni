#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DLZKA_MENA 50

// struktura pre spajany zoznam
struct Vsetky_Udaje {
    char meno[MAX_DLZKA_MENA];
    int pocet;
    struct Vsetky_Udaje* next;
};

void pridaj_osobu(struct Vsetky_Udaje** head, char* meno) {
    struct Vsetky_Udaje* current = *head;
    struct Vsetky_Udaje* prev = NULL;

    // hladaj meno v zozname
    while (current != NULL) {
        if (strcmp(current->meno, meno) == 0) {
            current->pocet++;
            return;
        }
        prev = current;
        current = current->next;
    }

    // ak meno neexistuje, pridaj novy uzol
    struct Vsetky_Udaje* novyUzol = malloc(sizeof(struct Vsetky_Udaje));
    if (!novyUzol) {
        fprintf(stderr, "Chyba: Nepodarilo sa alokovať pamäť pre nový uzol.\n");
        exit(1);
    }

    strcpy(novyUzol->meno, meno);
    novyUzol->pocet = 1;
    novyUzol->next = NULL;

    // pridaj novy uzol na koniec zoznamu
    if (prev == NULL) {
        *head = novyUzol;
    } else {
        prev->next = novyUzol;
    }
}

// hladanie uzla s najvacsou navstevnostou
struct Vsetky_Udaje* hladaj_maxLL(struct Vsetky_Udaje* head) {
    if (head == NULL) {
        return NULL;
    }

    struct Vsetky_Udaje* maxNode = head;
    struct Vsetky_Udaje* current = head->next;

    while (current != NULL) {
        if (current->pocet > maxNode->pocet) {
            maxNode = current;
        }
        current = current->next;
    }
    return maxNode;
}

// uvolnenie linked listu
void uvolni_zoznam(struct Vsetky_Udaje* head) {
    struct Vsetky_Udaje* current = head;
    while (current != NULL) {
        struct Vsetky_Udaje* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    struct Vsetky_Udaje* head = NULL;
    char meno[MAX_DLZKA_MENA];

    printf("Zadajte mena:\n");

    while (1) {
        scanf("%s", meno);

        // Kontrola ukončovacieho znaku
        if (meno[0] == '#' && meno[1] == '\0') {
            break;
        }

        // Pridaj alebo aktualizuj meno v zozname
        pridaj_osobu(&head, meno);
    }

    // Nájsť a vypísať meno s maximálnou návštevnosťou
    struct Vsetky_Udaje* maxNode = hladaj_maxLL(head);
    if (maxNode != NULL) {
        printf("Najcastejsie chodi do kina: %s\n", maxNode->meno);
    } else {
        printf("Zoznam je prazdny.\n");
    }

    uvolni_zoznam(head);

    return 0;
}
