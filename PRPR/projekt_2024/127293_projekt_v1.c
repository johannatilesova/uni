// Tilesova Johanna 127293

#include <stdio.h>
#include <string.h>

#define MAX_RIADKOV 100
#define MAX_DLZKA 100

// nacitanie suboru data.txt
int nacitaj_data_subor(const char* nazov_suboru, char hodnota1[][MAX_DLZKA], char hodnota2[][MAX_DLZKA], int* count) {
    FILE *subor = fopen(nazov_suboru, "r");
    if (subor == NULL) {
        return 0;  // chyba pri otvarani súboru
    }
    
    *count = 0;
    while (*count < MAX_RIADKOV && fscanf(subor, "%*d %*d %s %s", hodnota1[*count], hodnota2[*count]) == 2) {
        (*count)++;
    }
    fclose(subor);
    return 1;  // subor nacitany uspesne
}

// nacitanie suboru parse.txt
int nacitaj_parse_subor(const char* nazov_suboru, char poznamky[][MAX_DLZKA], int* count) {
    FILE *subor = fopen(nazov_suboru, "r");
    if (subor == NULL) {
        return 0;  // chyba pri otvarani súboru
    }
    
    *count = 0;
    while (fgets(poznamky[*count], MAX_DLZKA, subor) != NULL) {
        poznamky[*count][strcspn(poznamky[*count], "\n")] = 0;  // odstranenie znaku noveho riadku
        (*count)++;
    }
    fclose(subor);
    return 1;  // subor nacitany uspesne
}

// nacitanie suboru string.txt
int nacitaj_string_subor(const char* nazov_suboru, char strings[][MAX_DLZKA], int* count) {
    FILE *subor = fopen(nazov_suboru, "r");
    if (subor == NULL) {
        return 0;  // chyba pri otvarani súboru
    }
    
    *count = 0;
    while (fgets(strings[*count], MAX_DLZKA, subor) != NULL) {
        strings[*count][strcspn(strings[*count], "\n")] = 0;  // odstranenie znaku noveho riadku
        (*count)++;
    }
    fclose(subor);
    return 1;  // subor nacitany uspesne
}

// funkcia na spracovanie príkazu v 1
void funkcia_v1(int* otvorene_subory, int* data_count, int* string_count, 
                char hodnota1[][MAX_DLZKA], char hodnota2[][MAX_DLZKA], char poznamky[][MAX_DLZKA], char strings[][MAX_DLZKA]) {
    if (!*otvorene_subory) {
        if (!nacitaj_data_subor("data.txt", hodnota1, hodnota2, data_count) ||
            !nacitaj_parse_subor("parse.txt", poznamky, data_count) ||
            !nacitaj_string_subor("string.txt", strings, string_count)) {
            printf("V1: Neotvorene txt subory.\n");
            return;  // ak sa nepodarilo otvorit subory, tak koniec
        }
        *otvorene_subory = 1;
    }
    
    // vypis zaznamov
    for (int i = 0; i < *data_count; i++) {
        printf("ID. mer. modulu: %s\n", strings[i][0] == '-' ? "-" : strings[i]);
        printf("Hodnota 1: %s\n", hodnota1[i]);
        printf("Hodnota 2: %s\n", hodnota2[i]);
        printf("Poznámka: %s\n", poznamky[i]);
        printf("\n");
    }
}

// vypisanie histogramu znakov
void funkcia_h(char strings[][MAX_DLZKA], int string_count, int otvorene_subory) {
    if (!otvorene_subory) {
        printf("H: Neotvoreny subor.\n");
        return;
    }
    
    int histogram[256] = {0};  // pole na počítanie znakov

    // počítanie výskytu znakov
    for (int i = 0; i < string_count; i++) {
        for (int j = 0; j < strlen(strings[i]); j++) {
            unsigned char ch = strings[i][j];  // zmeníme typ na unsigned char
            histogram[ch]++;  // využijeme histogram priamo pre ch
        }
    }

    // výpis histogramu v poradí
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (histogram[(unsigned char)ch] > 0) {
            printf("%c : %d\n", ch, histogram[(unsigned char)ch]);
        }
    }

    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (histogram[(unsigned char)ch] > 0) {
            printf("%c : %d\n", ch, histogram[(unsigned char)ch]);
        }
    }

    for (char ch = '0'; ch <= '9'; ch++) {
        if (histogram[(unsigned char)ch] > 0) {
            printf("%c : %d\n", ch, histogram[(unsigned char)ch]);
        }
    }
}

void koniec(int* otvorene_subory) {
    if (*otvorene_subory) {
        *otvorene_subory = 0;
    }
}

int main() {
    char znak;
    int moznost;
    
    int otvorene_subory = 0;  // ukazatel na to, ci subory uz boli otvorene
    int data_count = 0, string_count = 0;
    
    char hodnota1[MAX_RIADKOV];  // Hodnota 1
    char hodnota2[MAX_RIADKOV];  // Hodnota 2
    char poznamky[MAX_RIADKOV][MAX_DLZKA];  // Poznamka z parse.txt
    char strings[MAX_RIADKOV][MAX_DLZKA];  // ID meracieho modulu zo string.txt

    while (1) {
        //printf("Zadaj príkaz: ");
        scanf(" %c", &znak);
        
        switch (znak) {
            case 'v':
                scanf(" %d", &moznost);  // nacitanie voľby po prikaze v
                
                if (moznost == 1) {
                    funkcia_v1(&otvorene_subory, &data_count, &string_count, &hodnota1, &hodnota2, poznamky, strings);
                } 
                else {
                    printf("V: Nesprávne volba vypisu. \n");
                }
                break;
            
            case 'h':
                funkcia_h(strings, string_count, otvorene_subory);
                break;
            
            case 'k':
                koniec(&otvorene_subory);
                break;
            
            
        }
    }
    
    return 0;
}