#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DLZKA 1000
#define MAX_RIADKOV 1000

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
        for (j = 0; j < strlen(strings[i]); j++) {
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

    uvolni_pole(hodnota1, *pocitadlo_data);
    uvolni_pole(hodnota2, *pocitadlo_data);
    uvolni_pole(poznamky, *pocitadlo_data);
    uvolni_pole(strings, *pocitadlo_string);

    *hodnota1 = (char **)malloc(new_data_count * sizeof(char *));
    *hodnota2 = (char **)malloc(new_data_count * sizeof(char *));
    *poznamky = (char **)malloc(new_parse_count * sizeof(char *));
    *strings = (char **)malloc(new_string_count * sizeof(char *));

    if (!*hodnota1 || !*hodnota2 || !*poznamky || !*strings) {
        return;
    }

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

    if (!subor_data || !subor_parse || !subor_string) {
        if (subor_data) fclose(subor_data);
        if (subor_parse) fclose(subor_parse);
        if (subor_string) fclose(subor_string);
        return;
    }

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
    
    if (!otvorene_subory || !spustene_n) {
        printf("Q: Polia nie su vytvorene.\n");
        return;
    }

    scanf("%d", &Y);
    Y--;

    if (Y < 0) {
        Y = 0;
    }

    if (Y > *pocitadlo_data) {
        Y = *pocitadlo_data;
    }
    
    getchar();
    fgets(new_string, MAX_DLZKA, stdin);
    new_string[strcspn(new_string, "\r\n")] = 0;
    scanf("%d %d %d %f", &num1, &num2, &num3, &num4);
    
    getchar();
    fgets(new_parse, MAX_DLZKA, stdin);
    new_parse[strcspn(new_parse, "\r\n")] = 0;
    
    *pocitadlo_data += 1;
    *pocitadlo_string += 1;

    *hodnota1 = (char **)realloc(*hodnota1, *pocitadlo_data * sizeof(char *));
    *hodnota2 = (char **)realloc(*hodnota2, *pocitadlo_data * sizeof(char *));
    *poznamky = (char **)realloc(*poznamky, *pocitadlo_data * sizeof(char *));
    *strings = (char **)realloc(*strings, *pocitadlo_string * sizeof(char *));
    
    for (i = *pocitadlo_data - 1; i > Y; i--) {
        (*hodnota1)[i] = (*hodnota1)[i - 1];
        (*hodnota2)[i] = (*hodnota2)[i - 1];
        (*poznamky)[i] = (*poznamky)[i - 1];
        (*strings)[i] = (*strings)[i - 1];
    }

    (*hodnota1)[Y] = (char *)malloc(MAX_DLZKA);
    (*hodnota2)[Y] = (char *)malloc(MAX_DLZKA);
    (*poznamky)[Y] = (char *)malloc(MAX_DLZKA);
    (*strings)[Y] = (char *)malloc(MAX_DLZKA);
    
    if (!(*hodnota1)[Y] || !(*hodnota2)[Y] || !(*poznamky)[Y] || !(*strings)[Y]) {
        return;
    }

    sprintf((*hodnota1)[Y], "%d", num3);
    sprintf((*hodnota2)[Y], "%g", num4);

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
    id[strcspn(id, "\r\n")] = 0;

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

            (*pocitadlo_data)--;
            (*pocitadlo_string)--;
            pocet_vymazanych++;
        } else {
            i++;
        }
    }

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

    for (i = 0; i < pocitadlo_data; i++) {
        if (strstr(poznamky[i], hladane_slovo) != NULL) {
            printf("%s\n", poznamky[i]);
            najdene_slovo = 1;
        }
    }

    if (!najdene_slovo) {
        printf("Neboli najdene_slovo ziadne zaznamy.\n");
    }
}

void funkcia_k(char ***hodnota1, char ***hodnota2, char ***poznamky, char ***strings,
               int *pocitadlo_data, int *pocitadlo_string) {
    
    uvolni_pole(hodnota1, *pocitadlo_data);
    uvolni_pole(hodnota2, *pocitadlo_data);
    uvolni_pole(poznamky, *pocitadlo_data);
    uvolni_pole(strings, *pocitadlo_string);

    exit(0);
}

int main() {
    char znak;
    int moznost;
    int otvorene_subory = 0;
    int pocitadlo_data = 0, pocitadlo_string = 0;
    int spustene_n = 0;
    char **hodnota1 = NULL, **hodnota2 = NULL, **poznamky = NULL, **strings = NULL;
    
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
                } else {
                    printf("V: Nespravna volba.\n");
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
                funkcia_k(&hodnota1, &hodnota2, &poznamky, &strings, &pocitadlo_data, &pocitadlo_string);
                return 0;
            default:
                printf("Neplatny prikaz.\n");
                break;
        }
    }
}