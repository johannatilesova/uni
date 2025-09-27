// Tilesova Johanna 127293

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DLZKA 1000
#define MAX_RIADKOV 1000

typedef struct Data {
    int Hodnota_ID;
    int Hodnota_zn;
    int Hodnota_1;
    float Hodnota_2;
} DATA;

typedef struct Parse {
    char Poznamka_ID[MAX_DLZKA];
    float Poznamka_N1;
    int Poznamka_Hodina;
    int Poznamka_Minuta;
    char Poznamka_T[MAX_DLZKA];
} PARSE;

typedef struct VsetkyUdaje {
    DATA data;
    PARSE parse;
    char IDmodulu[100];
    struct VsetkyUdaje* next;
} VSETKY_UDAJE;

void uvolni_spajany_zoznam(VSETKY_UDAJE** head) {
    VSETKY_UDAJE* current = *head;
    VSETKY_UDAJE* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

int nacitaj_pocet_riadkov(const char* filename) {
    int pocet_riadkov = 0;

    char ch;
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            pocet_riadkov++;
        }
    }

    fclose(file);
    return pocet_riadkov;
}

// vsetky subory sa otvaraju naraz v jednej funkcii
int nacitaj_subory(const char* data_filename, const char* parse_filename, const char* string_filename,
                   char ***hodnota1, char ***hodnota2, char ***poznamky, char ***strings,
                   int *pocitadlo_data, int *pocitadlo_string) {
    
    int pocitadlo_parse;
    int i, j;

    FILE *subor_data = NULL;
    FILE *subor_parse = NULL;
    FILE *subor_string = NULL;

    *pocitadlo_data = nacitaj_pocet_riadkov(data_filename);
    pocitadlo_parse = nacitaj_pocet_riadkov(parse_filename);
    *pocitadlo_string = nacitaj_pocet_riadkov(string_filename);

    if (*pocitadlo_data < 0 || pocitadlo_parse < 0 || *pocitadlo_string < 0) {
        return 0;
    }

    *hodnota1 = (char **)malloc(*pocitadlo_data * sizeof(char *));
    *hodnota2 = (char **)malloc(*pocitadlo_data * sizeof(char *));
    *poznamky = (char **)malloc(pocitadlo_parse * sizeof(char *));
    *strings = (char **)malloc(*pocitadlo_string * sizeof(char *));

    if (!*hodnota1 || !*hodnota2 || !*poznamky || !*strings) {
        free(*hodnota1); free(*hodnota2); free(*poznamky); free(*strings);
        return 0;
    }

    for (i = 0; i < *pocitadlo_data; i++) {
        (*hodnota1)[i] = (char *)malloc(MAX_DLZKA);
        (*hodnota2)[i] = (char *)malloc(MAX_DLZKA);

        if (!(*hodnota1)[i] || !(*hodnota2)[i]) {
            for (j = 0; j <= i; j++) {
                free((*hodnota1)[j]);
                free((*hodnota2)[j]);
            }
            free(*hodnota1); free(*hodnota2); free(*poznamky); free(*strings);
            return 0;
        }
    }

    for (i = 0; i < pocitadlo_parse; i++) {
        (*poznamky)[i] = (char *)malloc(MAX_DLZKA);
        if (!(*poznamky)[i]) {
            for (j = 0; j < *pocitadlo_data; j++) {
                free((*hodnota1)[j]);
                free((*hodnota2)[j]);
            }
            for (j = 0; j <= i; j++) {
                free((*poznamky)[j]);
            }
            free(*hodnota1); free(*hodnota2); free(*poznamky); free(*strings);
            return 0;
        }
    }

    for (i = 0; i < *pocitadlo_string; i++) {
        (*strings)[i] = (char *)malloc(MAX_DLZKA);
        if (!(*strings)[i]) {
            for (j = 0; j < *pocitadlo_data; j++) {
                free((*hodnota1)[j]);
                free((*hodnota2)[j]);
            }
            for (j = 0; j < pocitadlo_parse; j++) {
                free((*poznamky)[j]);
            }
            for (j = 0; j <= i; j++) {
                free((*strings)[j]);
            }
            free(*hodnota1); free(*hodnota2); free(*poznamky); free(*strings);
            return 0;
        }
    }

    subor_data = fopen(data_filename, "r");
    subor_parse = fopen(parse_filename, "r");
    subor_string = fopen(string_filename, "r");

    if (!subor_data || !subor_parse || !subor_string) {
        for (i = 0; i < *pocitadlo_data; i++) {
            free((*hodnota1)[i]);
            free((*hodnota2)[i]);
        }
        for (i = 0; i < pocitadlo_parse; i++) {
            free((*poznamky)[i]);
        }
        for (i = 0; i < *pocitadlo_string; i++) {
            free((*strings)[i]);
        }
        free(*hodnota1); free(*hodnota2); free(*poznamky); free(*strings);
        return 0;
    }

    for (i = 0; i < *pocitadlo_data; i++) {
        fscanf(subor_data, "%*d %*d %s %s", (*hodnota1)[i], (*hodnota2)[i]);
    }
    fclose(subor_data);

    for (i = 0; i < pocitadlo_parse; i++) {
        fgets((*poznamky)[i], MAX_DLZKA, subor_parse);
        (*poznamky)[i][strcspn((*poznamky)[i], "\r\n")] = 0;
    }
    fclose(subor_parse);

    for (i = 0; i < *pocitadlo_string; i++) {
        fgets((*strings)[i], MAX_DLZKA, subor_string);
        (*strings)[i][strcspn((*strings)[i], "\r\n")] = 0;
    }
    fclose(subor_string);

    return 1;
}

void funkcia_v1(int *otvorene_subory, int *pocitadlo_data, int *pocitadlo_string,
                char **hodnota1, char **hodnota2, char **poznamky, char **strings) {
    int i;

    if (!*otvorene_subory) {
        printf("V1: Neotvorene txt subory.\n");
        return;
    }

    for (i = 0; i < *pocitadlo_data; i++) {
        printf("ID. mer. modulu: %s\n", strings[i][0] == '-' ? "-" : strings[i]);
        printf("Hodnota 1: %s\n", hodnota1[i]);
        printf("Hodnota 2: %s\n", hodnota2[i]);
        printf("Poznamka: %s\n", poznamky[i]);
        printf("\n");
    }
}

void funkcia_v2(char **hodnota1, char **hodnota2, char **poznamky, char **strings,
                int pocitadlo_data, int spustene_n) {
    int i;

    // kontroluje, ci boli vytvorene polia
    if (!spustene_n) {
        printf("V2: Nenaplnene polia.\n");
        return;
    }

    for (i = 0; i < pocitadlo_data; i++) {
        printf("ID. mer. modulu: %s\n", strings[i]);
        printf("Hodnota 1: %s\n", hodnota1[i]);
        printf("Hodnota 2: %s\n", hodnota2[i]);
        printf("Poznamka: %s\n", poznamky[i]);
        printf("\n");
    }
}

void funkcia_v3(VSETKY_UDAJE* vsetky_data) {
    if (vsetky_data == NULL) {
        printf("V3: Nenaplneny spajany zoznam.\n");
        return;
    }

    VSETKY_UDAJE* current = vsetky_data;
    while (current != NULL) {
        if (strcmp(current->IDmodulu, "NaN") == 0 || strlen(current->IDmodulu) == 0) {
            printf("ID. mer. modulu: -\n");
        } else {
            printf("ID. mer. modulu: %s\n", current->IDmodulu);
        }

        printf("Hodnota 1: %d\n", current->data.Hodnota_1);
        
        if (current->data.Hodnota_2 == 0.0f){
            printf("Hodnota 2: %.1f\n", current->data.Hodnota_2);
        } else{
            printf("Hodnota 2: %g\n", current->data.Hodnota_2);
        }

        if (strcmp(current->parse.Poznamka_ID, "NaN") == 0 || strlen(current->parse.Poznamka_ID) == 0) {
            printf("Poznamka ID: NaN\n");
        } else {
            printf("Poznamka ID: %s\n", current->parse.Poznamka_ID);
        }

        if (current->parse.Poznamka_Hodina == -1 || current->parse.Poznamka_Minuta == -1) {
            printf("Poznamka C: -1 : -1 => -1\n");
        } else {
            printf("Poznamka C: %d : %d => %g\n", current->parse.Poznamka_Hodina, current->parse.Poznamka_Minuta, current->parse.Poznamka_N1);
        }

        if (strcmp(current->parse.Poznamka_T, "NaN") == 0 || strlen(current->parse.Poznamka_T) == 0) {
            printf("Poznamka T: NaN\n");
        } else {
            printf("Poznamka T: %s\n", current->parse.Poznamka_T);
        }

        printf("\n");

        current = current->next;
    }
}

void funkcia_h(char **strings, int pocitadlo_string, int otvorene_subory) {
    int histogram[256] = {0};
    unsigned char ch;
    char ch_out;
    int i, j;

    if (!otvorene_subory || !strings) {
        printf("H: Neotvoreny subor.\n");
        return;
    }

    for (i = 0; i < pocitadlo_string; i++) {
        for (j = 0; j < strlen(strings[i]); j++) { // prechadza znaky v retazci bez posledneho znaku '\0'
            ch = strings[i][j];
            histogram[ch]++;
        }
    }

    for (ch_out = 'A'; ch_out <= 'Z'; ch_out++) {
        if (histogram[(unsigned char)ch_out] > 0) {
            printf("%c : %d\n", ch_out, histogram[(unsigned char)ch_out]);
        }
    }

    for (ch_out = 'a'; ch_out <= 'z'; ch_out++) {
        if (histogram[(unsigned char)ch_out] > 0) {
            printf("%c : %d\n", ch_out, histogram[(unsigned char)ch_out]);
        }
    }

    for (ch_out = '0'; ch_out <= '9'; ch_out++) {
        if (histogram[(unsigned char)ch_out] > 0) {
            printf("%c : %d\n", ch_out, histogram[(unsigned char)ch_out]);
        }
    }
}

void uvolni_pole(char ***pole, int pocet) {
    int i;

    if (*pole) {
        for (i = 0; i < pocet; i++) {
            free((*pole)[i]);
        }
        free(*pole);
    }
}

void funkcia_n(char ***hodnota1, char ***hodnota2, char ***poznamky, char ***strings,
               int *pocitadlo_data, int *pocitadlo_string, int *spustene_n, int otvorene_subory) {
    int i;

    int new_data_count = nacitaj_pocet_riadkov("data.txt");
    int new_parse_count = nacitaj_pocet_riadkov("parse.txt");
    int new_string_count = nacitaj_pocet_riadkov("string.txt");

    FILE *subor_data = fopen("data.txt", "r");
    FILE *subor_parse = fopen("parse.txt", "r");
    FILE *subor_string = fopen("string.txt", "r");

    if (!otvorene_subory) {
        printf("N: Neotvoreny subor.\n");
        return;
    }
    if (new_data_count < 0 || new_parse_count < 0 || new_string_count < 0) {
        return;
    }

    // uvolnenie pamate pre vsetky polia
    uvolni_pole(hodnota1, *pocitadlo_data);
    uvolni_pole(hodnota2, *pocitadlo_data);
    uvolni_pole(poznamky, *pocitadlo_data);
    uvolni_pole(strings, *pocitadlo_string);

    // alokovanie novej pamate pre polia
    *hodnota1 = (char **)malloc(new_data_count * sizeof(char *));
    *hodnota2 = (char **)malloc(new_data_count * sizeof(char *));
    *poznamky = (char **)malloc(new_parse_count * sizeof(char *));
    *strings = (char **)malloc(new_string_count * sizeof(char *));

    if (!*hodnota1 || !*hodnota2 || !*poznamky || !*strings) {
        return;
    }

    // alokovanie pamate pre kazdy retazec
    for (i = 0; i < new_data_count; i++) {
        (*hodnota1)[i] = (char *)malloc(MAX_DLZKA);
        (*hodnota2)[i] = (char *)malloc(MAX_DLZKA);
    }
    for (i = 0; i < new_parse_count; i++) {
        (*poznamky)[i] = (char *)malloc(MAX_DLZKA);
    }
    for (i = 0; i < new_string_count; i++) {
        (*strings)[i] = (char *)malloc(MAX_DLZKA);
    }

    // kontrola, ci sa podarilo alokovat pamat
    if (!subor_data || !subor_parse || !subor_string) {
        if (subor_data) fclose(subor_data);
        if (subor_parse) fclose(subor_parse);
        if (subor_string) fclose(subor_string);
        return;
    }

    // nacita data zo súborov
    for (i = 0; i < new_data_count; i++) {
        fscanf(subor_data, "%*d %*d %s %s", (*hodnota1)[i], (*hodnota2)[i]);
    }
    fclose(subor_data);

    for (i = 0; i < new_parse_count; i++) {
        fgets((*poznamky)[i], MAX_DLZKA, subor_parse);
        (*poznamky)[i][strcspn((*poznamky)[i], "\r\n")] = 0;
    }
    fclose(subor_parse);

    for (i = 0; i < new_string_count; i++) {
        fgets((*strings)[i], MAX_DLZKA, subor_string);
        (*strings)[i][strcspn((*strings)[i], "\r\n")] = 0;
    }
    fclose(subor_string);

    *pocitadlo_data = new_data_count;
    *pocitadlo_string = new_string_count;
    *spustene_n = 1;
}

void funkcia_q(char ***hodnota1, char ***hodnota2, char ***poznamky, char ***strings,
               int *pocitadlo_data, int *pocitadlo_string, int otvorene_subory, int spustene_n) {
    int Y;
    int i;
    char new_string[MAX_DLZKA];
    int num1, num2, num3;
    float num4;
    char new_parse[MAX_DLZKA];
    
    // kontrola, ci su vytvorene polia
    if (!otvorene_subory || !spustene_n) {
        printf("Q: Polia nie su vytvorene.\n");
        return;
    }

    scanf("%d", &Y);
    Y--; // zmena indexovania od 1 na indexovanie od 0

    // ak je Y mimo rozsah, nastavi Y na posledny index
    if (Y < 0) {
        Y = 0;
    }

    if (Y > *pocitadlo_data) {
        Y = *pocitadlo_data;
    }
    
    // nacitanie hodnot a odstranenie znaku konca riadku
    getchar();
    fgets(new_string, MAX_DLZKA, stdin);
    new_string[strcspn(new_string, "\r\n")] = 0;
    scanf("%d %d %d %f", &num1, &num2, &num3, &num4);
    
    // nacitanie poznamky a odstranenie znaku konca riadku
    getchar();
    fgets(new_parse, MAX_DLZKA, stdin);
    new_parse[strcspn(new_parse, "\r\n")] = 0;
    
    // navysenie poctu zaznamov a stringov, kvoli realokacii pamate
    *pocitadlo_data += 1;
    *pocitadlo_string += 1;

    // realokacia pamate pre polia
    *hodnota1 = (char **)realloc(*hodnota1, *pocitadlo_data * sizeof(char *));
    *hodnota2 = (char **)realloc(*hodnota2, *pocitadlo_data * sizeof(char *));
    *poznamky = (char **)realloc(*poznamky, *pocitadlo_data * sizeof(char *));
    *strings = (char **)realloc(*strings, *pocitadlo_string * sizeof(char *));
    
    // posunutie zaznamov o jedno do prava, lebo sme vlozili novy zaznam
    for (i = *pocitadlo_data - 1; i > Y; i--) {
        (*hodnota1)[i] = (*hodnota1)[i - 1];
        (*hodnota2)[i] = (*hodnota2)[i - 1];
        (*poznamky)[i] = (*poznamky)[i - 1];
        (*strings)[i] = (*strings)[i - 1];
    }

    // alokacia pamate pre novy zaznam
    (*hodnota1)[Y] = (char *)malloc(MAX_DLZKA);
    (*hodnota2)[Y] = (char *)malloc(MAX_DLZKA);
    (*poznamky)[Y] = (char *)malloc(MAX_DLZKA);
    (*strings)[Y] = (char *)malloc(MAX_DLZKA);
    
    // kontrola, ci sa podarilo alokovat pamat
    if (!(*hodnota1)[Y] || !(*hodnota2)[Y] || !(*poznamky)[Y] || !(*strings)[Y]) {
        return;
    }

    // zapis hodnot do poli
    sprintf((*hodnota1)[Y], "%d", num3);
    sprintf((*hodnota2)[Y], "%g", num4); // zapis hodnoty s desatinnou ciarkou

    // zapis poznamky a id do poli
    strcpy((*poznamky)[Y], new_parse);
    strcpy((*strings)[Y], new_string);
}

void funkcia_w(char ***hodnota1, char ***hodnota2, char ***poznamky, char ***strings,
               int *pocitadlo_data, int *pocitadlo_string, int otvorene_subory, int spustene_n) {
    
    char id[MAX_DLZKA];
    int pocet_vymazanych = 0;
    int i, j;

    if (!otvorene_subory || !spustene_n) {
        printf("W: Polia nie su vytvorene.\n");
        return;
    }

    scanf("%s", id);
    id[strcspn(id, "\r\n")] = 0; // odstranenie znaku konca riadku

    // posunie zaznamy o jedno do lava, ak sa vymaze zaznam
    for (i = 0; i < *pocitadlo_data; ) {
        if (strcmp((*strings)[i], id) == 0) {
            free((*hodnota1)[i]);
            free((*hodnota2)[i]);
            free((*poznamky)[i]);
            free((*strings)[i]);
            
            for (j = i; j < *pocitadlo_data - 1; j++) {
                (*hodnota1)[j] = (*hodnota1)[j + 1];
                (*hodnota2)[j] = (*hodnota2)[j + 1];
                (*poznamky)[j] = (*poznamky)[j + 1];
                (*strings)[j] = (*strings)[j + 1];
            }
            // znizenie poctu zaznamov a stringov
            (*pocitadlo_data)--;
            (*pocitadlo_string)--;
            pocet_vymazanych++; // inkrementacia poctu vymazanych zaznamov
        } else {
            i++;
        }
    }

    // realokacia pamate pre polia
    *hodnota1 = (char **)realloc(*hodnota1, *pocitadlo_data * sizeof(char *));
    *hodnota2 = (char **)realloc(*hodnota2, *pocitadlo_data * sizeof(char *));
    *poznamky = (char **)realloc(*poznamky, *pocitadlo_data * sizeof(char *));
    *strings = (char **)realloc(*strings, *pocitadlo_string * sizeof(char *));
    printf("W: Vymazalo sa : %d zaznamov !\n", pocet_vymazanych);
}

void funkcia_e(char **poznamky, int pocitadlo_data, int otvorene_subory, int spustene_n) {
    char hladane_slovo[MAX_DLZKA];
    int najdene_slovo = 0;
    int i;

    if (!otvorene_subory || !spustene_n) {
        printf("E: Polia nie su vytvorene.\n");
        return;
    }

    scanf("%s", hladane_slovo);
    hladane_slovo[strcspn(hladane_slovo, "\r\n")] = 0;

    // prechadza vsetky polia a hlada zadane slovo v kazdej poznamke
    for (i = 0; i < pocitadlo_data; i++) {
        if (strstr(poznamky[i], hladane_slovo) != NULL) {
            printf("%s\n", poznamky[i]); // printne poznamky kde sa naslo hladane slovo
            najdene_slovo = 1;
        }
    }

    // ak sa nenajde hladane slovo
    if (!najdene_slovo) {
        printf("Neboli najdene_slovo ziadne zaznamy.\n");
    }
}

void rozdel_poznamku(char* riadok, PARSE* parse) {
    char *hastag;
    char temp[100]; // pomocna premenna pre ukladanie hodnot

    // na zaciatku inicializujeme na defaultne hodnoty
    strcpy(parse->Poznamka_ID, "NaN");
    parse->Poznamka_N1 = -1;
    parse->Poznamka_Hodina = -1;
    parse->Poznamka_Minuta = -1;
    strcpy(parse->Poznamka_T, "NaN");

    char *hastags[4] = {NULL, NULL, NULL, NULL};
    int i = 0;
    hastag = strtok(riadok, "#");
    while (hastag != NULL && i < 4) {
        hastags[i++] = hastag;
        hastag = strtok(NULL, "#");
    }

    if (hastags[0] && strlen(hastags[0]) > 0) {
        strncpy(parse->Poznamka_ID, hastags[0], sizeof(parse->Poznamka_ID) - 1);
        parse->Poznamka_ID[sizeof(parse->Poznamka_ID) - 1] = '\0';
    }

    if (hastags[1] && strlen(hastags[1]) > 0) {
        parse->Poznamka_N1 = atof(hastags[1]);
    }

    if (hastags[2] && strlen(hastags[2]) >= 4) {
        strncpy(temp, hastags[2], 2);
        temp[2] = '\0';
        parse->Poznamka_Hodina = atoi(temp);

        strncpy(temp, hastags[2] + 2, 2);
        temp[2] = '\0';
        parse->Poznamka_Minuta = atoi(temp);
    }

    if (hastags[3] && strlen(hastags[3]) > 0) {
        strncpy(parse->Poznamka_T, hastags[3], sizeof(parse->Poznamka_T) - 1);
        parse->Poznamka_T[sizeof(parse->Poznamka_T) - 1] = '\0';
    }
}

int funkcia_m(const char* data_filename, const char* parse_filename, const char* string_filename, VSETKY_UDAJE** vsetky_data, int otvorene_subory) {
    if (!otvorene_subory) {
        printf("M: Neotvoreny subor.\n");
        return 0;
    }

    FILE* subor_data = fopen(data_filename, "r");
    FILE* subor_parse = fopen(parse_filename, "r");
    FILE* subor_string = fopen(string_filename, "r");

    if (!subor_data || !subor_parse || !subor_string) {
        printf("M: Neotvoreny subor.\n");
        if (subor_data) fclose(subor_data);
        if (subor_parse) fclose(subor_parse);
        if (subor_string) fclose(subor_string);
        return 0;
    }

    // ak bol zoznam naplneny, tak sa uvolni
    uvolni_spajany_zoznam(vsetky_data);

    VSETKY_UDAJE* current = NULL;
    int pocet_zaznamov = 0;
    char zaznam_data[100], zaznam_parse[MAX_DLZKA], zaznam_string[100];

    // nacitavanie udajov zo suborov
    while (fgets(zaznam_data, sizeof(zaznam_data), subor_data) &&
           fgets(zaznam_parse, sizeof(zaznam_parse), subor_parse) &&
           fgets(zaznam_string, sizeof(zaznam_string), subor_string)) {
        
        zaznam_parse[strcspn(zaznam_parse, "\r\n")] = '\0';
        zaznam_string[strcspn(zaznam_string, "\r\n")] = '\0';

        // ak je 'zaznam_parse' prázdny alebo obsahuje len oddelovače, priradíme predvolené hodnoty
        if (strcmp(zaznam_parse, "####") == 0) {
            strcpy(zaznam_parse, "NaN#-1#-1-1#NaN");
        }

        VSETKY_UDAJE* new_node = (VSETKY_UDAJE*)malloc(sizeof(VSETKY_UDAJE));
        if (!new_node) {
            printf("Chyba alokacie pamate\n");
            break;
        }

        sscanf(zaznam_data, "%d %d %d %f", &new_node->data.Hodnota_ID, &new_node->data.Hodnota_zn, &new_node->data.Hodnota_1, &new_node->data.Hodnota_2);

        // nacitanie a rozdelenie poznamky zo súboru parse.txt
        rozdel_poznamku(zaznam_parse, &new_node->parse);

        // nacitanie údajov zo súboru string.txt
        if (strlen(zaznam_string) > 0) {
            strncpy(new_node->IDmodulu, zaznam_string, sizeof(new_node->IDmodulu) - 1);
            new_node->IDmodulu[sizeof(new_node->IDmodulu) - 1] = '\0';
        } else {
            strcpy(new_node->IDmodulu, "-");
        }

        new_node->next = NULL;
        if (*vsetky_data == NULL) {
            *vsetky_data = new_node;
        } else {
            current->next = new_node;
        }
        current = new_node;
        pocet_zaznamov++;
    }

    fclose(subor_data);
    fclose(subor_parse);
    fclose(subor_string);

    printf("M: Nacitalo sa %d zaznamov.\n", pocet_zaznamov);
    return 1;
}

void funkcia_k(char ***hodnota1, char ***hodnota2, char ***poznamky, char ***strings,
               int *pocitadlo_data, int *pocitadlo_string, VSETKY_UDAJE** vsetky_data) {
    if (*hodnota1) {
        uvolni_pole(hodnota1, *pocitadlo_data);
    }
    if (*hodnota2) {
        uvolni_pole(hodnota2, *pocitadlo_data);
    }
    if (*poznamky) {
        uvolni_pole(poznamky, *pocitadlo_data);
    }
    if (*strings) {
        uvolni_pole(strings, *pocitadlo_string);
    }

    // uvolenenie spjaaneho zoznamu
    if (*vsetky_data) {
        uvolni_spajany_zoznam(vsetky_data);
    }

    // ukoncenie programu
    exit(0);
}

void funkcia_a(VSETKY_UDAJE** vsetky_data) {
    int Y;
    scanf("%d", &Y);
    getchar(); // odstrani sa znak nového riadku po zadaní čísla

    VSETKY_UDAJE* new_node = (VSETKY_UDAJE*)malloc(sizeof(VSETKY_UDAJE));
    if (!new_node) {
        printf("Chyba alokacie pamate\n");
        return;
    }

    // nacitanie udajov zo vstupu
    fgets(new_node->IDmodulu, sizeof(new_node->IDmodulu), stdin);
    new_node->IDmodulu[strcspn(new_node->IDmodulu, "\r\n")] = '\0';

    scanf("%d %d %d %f", &new_node->data.Hodnota_ID, &new_node->data.Hodnota_zn, &new_node->data.Hodnota_1, &new_node->data.Hodnota_2);
    getchar(); // odstráni znak nového riadku po zadaní cisel

    char zaznam_parse[MAX_DLZKA];
    fgets(zaznam_parse, sizeof(zaznam_parse), stdin);
    zaznam_parse[strcspn(zaznam_parse, "\r\n")] = '\0';
    rozdel_poznamku(zaznam_parse, &new_node->parse);

    new_node->next = NULL;

    // pridanie záznamu na pozíciu Y
    if (*vsetky_data == NULL || Y <= 1) {
        // pridaj na začiatok zoznamu
        new_node->next = *vsetky_data;
        *vsetky_data = new_node;
    } else {
        VSETKY_UDAJE* current = *vsetky_data;
        int index = 1;

        // hladanie poslednej pozície
        while (current->next != NULL && index < Y - 1) {
            current = current->next;
            index++;
        }

        // pridanie na danú pozíciu
        new_node->next = current->next;
        current->next = new_node;
    }
}

void funkcia_s(VSETKY_UDAJE** vsetky_data) {
    if (*vsetky_data == NULL) {
        printf("S: Spajany zoznam nie je vytvoreny.\n");
        return;
    }

    char id[100];
    scanf("%s", id);

    VSETKY_UDAJE* current = *vsetky_data;
    VSETKY_UDAJE* previous = NULL;
    int pocet_vymazanych = 0;

    while (current != NULL) {
        if (strcmp(current->IDmodulu, id) == 0) {
            if (previous == NULL) {
                *vsetky_data = current->next;
            } else {
                previous->next = current->next;
            }
            VSETKY_UDAJE* temp = current;
            current = current->next;
            free(temp);
            pocet_vymazanych++;
        } else {
            previous = current;
            current = current->next;
        }
    }

    if (pocet_vymazanych > 0) {
        printf("S: Vymazalo sa : %d zaznamov !\n", pocet_vymazanych);
    } else {
        printf("S: Zaznam s ID '%s' nebol najdeny.\n", id);
    }
}

void funkcia_d(VSETKY_UDAJE** vsetky_data, int c1, int c2) {
    int index;

    if (*vsetky_data == NULL) {
        return;
    }

    if (c1 == c2) {
        return;
    }

    VSETKY_UDAJE *prev1 = NULL, *prev2 = NULL;
    VSETKY_UDAJE *node1 = *vsetky_data, *node2 = *vsetky_data;

    
    index = 1;
    while (node1 != NULL && index < c1) {
        prev1 = node1;
        node1 = node1->next;
        index++;
    }

    index = 1;
    while (node2 != NULL && index < c2) {
        prev2 = node2;
        node2 = node2->next;
        index++;
    }

    if (node1 == NULL || node2 == NULL) {
        return;
    }

    if (prev1 != NULL) {
        prev1->next = node2;
    } else {
        *vsetky_data = node2;
    }

    if (prev2 != NULL) {
        prev2->next = node1;
    } else {
        *vsetky_data = node1;
    }

    VSETKY_UDAJE* temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;
}

int main() {
    char znak;
    int moznost;
    int otvorene_subory = 0;
    int pocitadlo_data = 0, pocitadlo_string = 0;
    int spustene_n = 0;
    char **hodnota1 = NULL, **hodnota2 = NULL, **poznamky = NULL, **strings = NULL;

    VSETKY_UDAJE* vsetky_data = NULL;

    int c1, c2;

    while (1) {
        scanf(" %c", &znak);
        switch (znak) {
            case 'v':
                scanf(" %d", &moznost);
                if (!otvorene_subory) {
                    if (!nacitaj_subory("data.txt", "parse.txt", "string.txt", &hodnota1, &hodnota2, &poznamky, &strings, &pocitadlo_data, &pocitadlo_string)) {
                        printf("V1: Neotvorene txt subory.\n");
                        break;
                    }
                    otvorene_subory = 1;
                }
                if (moznost == 1) {
                    funkcia_v1(&otvorene_subory, &pocitadlo_data, &pocitadlo_string, hodnota1, hodnota2, poznamky, strings);
                } else if (moznost == 2) {
                    funkcia_v2(hodnota1, hodnota2, poznamky, strings, pocitadlo_data, spustene_n);
                } else if (moznost == 3) {
                    funkcia_v3(vsetky_data);
                } else {
                    printf("V: Nespravna volba vypisu.\n");
                }
                break;
            case 'h':
                funkcia_h(strings, pocitadlo_string, otvorene_subory);
                break;
            case 'n':
                funkcia_n(&hodnota1, &hodnota2, &poznamky, &strings, &pocitadlo_data, &pocitadlo_string, &spustene_n, otvorene_subory);
                break;
            case 'q':
                funkcia_q(&hodnota1, &hodnota2, &poznamky, &strings, &pocitadlo_data, &pocitadlo_string, otvorene_subory, spustene_n);
                break;
            case 'w':
                funkcia_w(&hodnota1, &hodnota2, &poznamky, &strings, &pocitadlo_data, &pocitadlo_string, otvorene_subory, spustene_n);
                break;
            case 'e':
                funkcia_e(poznamky, pocitadlo_data, otvorene_subory, spustene_n);
                break;
            case 'k':
                funkcia_k(&hodnota1, &hodnota2, &poznamky, &strings, &pocitadlo_data, &pocitadlo_string, &vsetky_data);
                return 0;
            case 'm':
                if (!funkcia_m("data.txt", "parse.txt", "string.txt", &vsetky_data, otvorene_subory)) {
                    printf("M: Neotvoreny subor.\n");
                }
                break;
            case 'a':
                funkcia_a(&vsetky_data);
                break;
            case 's':
                funkcia_s(&vsetky_data);
                break;
            case 'd':
                scanf("%d %d", &c1, &c2);
                funkcia_d(&vsetky_data, c1, c2);
                break;
            default:
                printf("Neplatny prikaz.\n");
                break;
        }
    }
}