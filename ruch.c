#include "struktury.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "zapisanie_i_wczytanie.h"

#define AI
#ifndef AI

int rusz(struct struktura_planszy *plansza, int *rezultat){
    while (1){
poczatek:
        ;
        int czy_mozliwy_ruch; //zmienia wartosc w jesli jest mozliwy ruch do wykoanania przez twojego pingwina
        czy_mozliwy_ruch = 0;
        for (int i = 0; i < plansza->liczba_wierszy; i++){//sprawdzanie czy jest na planszy twoj pingwin ktory moze wykonac ruch
            for (int j = 0; j < plansza->liczba_kolumn; j++){
                if (plansza->pingwiny_na_polu[i][j] == plansza->twoj_nr){//jesli znalazl pingwina na polu
                    if (((i + 1) < plansza->liczba_wierszy && plansza->ryby[i + 1][j] != 0) || //jesli istnieje pole na ktore moze poruszyc sie pingwin
                            ((i - 1) >= 0 && plansza->ryby[i - 1][j] != 0) ||
                            ((j + 1) < plansza->liczba_kolumn && plansza->ryby[i][j + 1] != 0) ||
                            ((j - 1) >= 0 && plansza->ryby[i][j - 1] != 0)){
                        czy_mozliwy_ruch = 1; //zmiana wartosci
                        goto ucieczka; //wyjscie z petli po znalezieniu pingwina ktory moze sie poruszyc
                    }
                }
            }
        }
ucieczka:
        ;
        if (czy_mozliwy_ruch == 0){//jesli ruch jest niemozliwy rezultat jest zmieniany na 1 i funkcja konczy dzialanie
            *rezultat = 1;
            break;
        }

        printf("Podaj wspolrzedne pingwina ktorym chcesz sie poruszyc\n");
        rysuj_plansze(*plansza);
        int x1, y1;
        scanf("%d %d", &y1, &x1); //zczytuje najpier numer kolumny, pozniej numer wiersza
        //2 kolejne ify sprawdzaja (1)czy wybrales pole nalezace do planszy i (2)czy na wybranym polu jest twoj pingwin
        if ((x1 - 1) < 0 || (x1 - 1)>(plansza->liczba_wierszy - 1) || (y1 - 1) < 0 || (y1 - 1)>(plansza->liczba_kolumn - 1)){//(1)
            printf("Podane współrzędne są poza zakresem planszy\n");
            continue;
        }
        if (plansza->pingwiny_na_polu[x1 - 1][y1 - 1] != plansza->twoj_nr){//(2)
            printf("Podane wspolrzedne nie naleza to twojego pingwina\n");
            continue;
        }
        printf("Podaj wspolrzedne pola na ktore chcesz sie poruszyc\n");
        int x2, y2;
        scanf("%d %d", &y2, &x2); //zczytuje najpier numer kolumny, pozniej numer wiersza
        /*Nastepne 8 if sprawdza czy: 
         * (1) pole na ktore chesz sie poruszyc znajduje sie poza plansza
         * (2) na polu na ktore chcesz sie poruszyc sa ryby
         * (3) pole na ktore chesz sie poruszyc rozni sie od pola na ktorym stoisz 2 wspolrzednymi(jest na skos)
         * (4) pole na ktore chesz sie poruszyc jest polem na ktorym juz stoisz
         * (5 - 8) sprawdza czy po drodze na wybrane pole nie ma pola na którym nie ma zadnych ryb 
         * (obejmuje to przypadek pola pustego i pola z pingwinem)         
         */
        if ((x2 - 1) < 0 || (x2 - 1)>(plansza->liczba_wierszy - 1) || (y2 - 1) < 0 || (y2 - 1)>(plansza->liczba_kolumn - 1)){//(1)
            printf("Podane współrzędne są poza zakresem planszy\n");
            continue;
        }
        if (plansza->ryby[x2 - 1][y2 - 1] == 0){//(2)
            printf("Nie można poruszyc sie na dane pole\n");
            continue;
        }
        if (x1 != x2 && y1 != y2){//(3)
            printf("Nie można poruszyc sie na dane pole\n");
            continue;
        }
        if (x1 == x2 && y1 == y2){//(4)
            printf("Juz stoisz na tym polu\n");
            continue;
        }
        if (y2 < y1){//(5)
            for (int i = y2; i < y1; i++){
                if (plansza->ryby[x1 - 1][i - 1] == 0){
                    printf("Ruch niedozwolony\n");
                    goto poczatek;
                }
            }
        }
        if (y2 > y1){//(6)
            for (int i = y1; i < y2; i++){
                if (plansza->ryby[x1 - 1][i] == 0){
                    printf("Ruch niedozwolony\n");
                    goto poczatek;
                }
            }
        }
        if (x2 < x1){//(7)
            for (int i = x2; i < x1; i++){
                if (plansza->ryby[i - 1][y1 - 1] == 0){
                    printf("Ruch niedozwolony\n");
                    goto poczatek;
                }
            }
        }
        if (x2 > x1){//(8)
            for (int i = x1; i < x2; i++){
                if (plansza->ryby[i][y1 - 1] == 0){
                    printf("Ruch niedozwolony\n");
                    goto poczatek;
                }
            }
        }
        plansza->pingwiny_na_polu[x1 - 1][y1 - 1] = 0;
        int punkty;
        punkty = plansza->ryby[x2 - 1][y2 - 1];
        plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
        plansza->ryby[x2 - 1][y2 - 1] = 0;
        plansza->pingwiny_na_polu[x2 - 1][y2 - 1] = plansza->twoj_nr;
        break;
    }
    return (0);
}

#else

struct odleglosc_od_krawedzi{//przechowuje odleglosc pingwina od 1 z 4 krawedzi planszy
    int odleglosc;
    char kierunek[51];
};


void znajdz_pole_dol(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc); //funkcja wykonuje ruch w danym kierunku o ile warunki sa spelnione
void znajdz_pole_gora(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc);
void znajdz_pole_prawo(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc);
void znajdz_pole_lewo(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc);
void bubbleSort(struct odleglosc_od_krawedzi priorytet_kierunkow[4], int n); //funkcja sortujaca rosnaco tablice struktur odleglosc_od_krawedzi według rozmiaru .odleglosc

int rusz(struct struktura_planszy *plansza, int *rezultat){
    while (1){
poczatek:
        ;
        int czy_mozliwy_ruch; //zmienia wartosc w jesli jest mozliwy ruch do wykoanania przez twojego pingwina
        czy_mozliwy_ruch = 0;
        for (int i = 0; i < plansza->liczba_wierszy; i++){//sprawdzanie czy jest na planszy twoj pingwin ktory moze wykonac ruch
            for (int j = 0; j < plansza->liczba_kolumn; j++){
                if (plansza->pingwiny_na_polu[i][j] == plansza->twoj_nr){//jesli znalazl pingwina na polu
                    if (((i + 1) < plansza->liczba_wierszy && plansza->ryby[i + 1][j] != 0) || //jesli istnieje pole na ktore moze poruszyc sie pingwin
                            ((i - 1) >= 0 && plansza->ryby[i - 1][j] != 0) ||
                            ((j + 1) < plansza->liczba_kolumn && plansza->ryby[i][j + 1] != 0) ||
                            ((j - 1) >= 0 && plansza->ryby[i][j - 1] != 0)){
                        czy_mozliwy_ruch = 1; //zmiana wartosci
                        goto ucieczka; //wyjscie z petli po znalezieniu pingwina ktory moze sie poruszyc
                    }
                }
            }
        }
ucieczka:
        ;
        if (czy_mozliwy_ruch == 0){//jesli ruch jest niemozliwy rezultat jest zmieniany na 1 i funkcja konczy dzialanie
            *rezultat = 1;
            break;
        }

        int ostatecznosc = 0; //jesli jest 0 to nie mozna stawaic pingwina obok swojego innego pingwina, jak 1 to mozna
glowna_petla:
        ;
        int max_dziury = 0; //oznacza maksymalna ilosc pol blokujacych ruch pingwina
        int dziura = 0; //oznacza ilosc pol blokujacych ruch pingwina
        int ryby = 3; //oznacza ile ryb musi byc na polu na ktore chcesz sie poruszyc

        struct odleglosc_od_krawedzi priorytet_kierunkow[4];
        strncpy(priorytet_kierunkow[0].kierunek, "dol", 51);
        strncpy(priorytet_kierunkow[1].kierunek, "prawo", 51);
        strncpy(priorytet_kierunkow[2].kierunek, "gora", 51);
        strncpy(priorytet_kierunkow[3].kierunek, "lewo", 51);

        for (int i = 0; i < plansza->liczba_wierszy; i++){//sprawdzanie czy jest na planszy twoj pingwin ktory moze wykonac ruch
            for (int j = 0; j < plansza->liczba_kolumn; j++){
                if (plansza->pingwiny_na_polu[i][j] == plansza->twoj_nr){//jesli znalazl pingwina na polu

                    strncpy(priorytet_kierunkow[0].kierunek, "dol", 51);
                    strncpy(priorytet_kierunkow[1].kierunek, "prawo", 51);
                    strncpy(priorytet_kierunkow[2].kierunek, "gora", 51);
                    strncpy(priorytet_kierunkow[3].kierunek, "lewo", 51);

                    priorytet_kierunkow[0].odleglosc = plansza->liczba_wierszy - 1 - i; //obliczanie odleglosci pingwina od 4 krawedzi planszy
                    priorytet_kierunkow[1].odleglosc = plansza->liczba_kolumn - 1 - j;
                    priorytet_kierunkow[2].odleglosc = 0 + i;
                    priorytet_kierunkow[3].odleglosc = 0 + j;

                    bubbleSort(priorytet_kierunkow, 4); //sortowanie tablicy struktur wedlug odleglosci od krawedzi
                    //określanie kierunku ktory najpierw ma byc zbadany pod wzgledem poruszenia sie(pierwszenstwo maja kierunki ktore sa najbardzeij oddalone od krawedzi)
                    //tzn jeśli pingwin najbardziej jest oddalony od krawedzi dolnej, program bedzie badal najpier czy ruch w kierunku dolu spelnia wyagania ruchu
                    for (int m = 0; m<4; m++){
                        if (strncmp(priorytet_kierunkow[m].kierunek, "dol", 51) == 0){
                            znajdz_pole_dol(plansza, i, j, &dziura, ryby, max_dziury, ostatecznosc);
                            if (dziura == -1){
                                goto koniec;
                            }
                        }
                        if (strncmp(priorytet_kierunkow[m].kierunek, "prawo", 51) == 0){
                            znajdz_pole_prawo(plansza, i, j, &dziura, ryby, max_dziury, ostatecznosc);
                            if (dziura == -1){
                                goto koniec;
                            }
                        }
                        if (strncmp(priorytet_kierunkow[m].kierunek, "gora", 51) == 0){
                            znajdz_pole_gora(plansza, i, j, &dziura, ryby, max_dziury, ostatecznosc);
                            if (dziura == -1){
                                goto koniec;
                            }
                        }
                        if (strncmp(priorytet_kierunkow[m].kierunek, "lewo", 51) == 0){
                            znajdz_pole_lewo(plansza, i, j, &dziura, ryby, max_dziury, ostatecznosc);
                            if (dziura == -1){
                                goto koniec;
                            }
                        }
                    }
                }
                if ((i == (plansza->liczba_wierszy - 1)) && (j == (plansza->liczba_kolumn - 1))){//jesli na planszy nie ma pola spelniajacego wymagania to obniza je i szuka ponownie
                    ryby--; //zmniejsza liczbe ryb wymbaganych na polu
                    if (ryby == 0 && max_dziury != 4){//zwieksza liczbe dziur mozliwych wokol pola
                        ryby = 3;
                        max_dziury++;

                    }
                    if (ryby == 0 && max_dziury == 4 && ostatecznosc == 0){//umozliwia stawianie pingwina obok twojego pingwina
                        ryby = 3;
                        ostatecznosc = 1;
                        goto glowna_petla;
                    }
                    if (ryby == 0 && max_dziury == 4 && ostatecznosc == 1){
                        ryby = 3;
                        max_dziury = 0;
                        goto koniec;
                    }
                    i = 0;
                    j = -1; //bo 1 zostanie dodane napoczątku pętli

                }
            }
        }

    }
koniec:
    return (0);
}

void znajdz_pole_dol(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc){

    if ((i + 1) < plansza->liczba_wierszy && plansza->ryby[i + 1][j] != 0){
        int x = i + 1;
        int y = j;
        for (int k = x; k < plansza->liczba_wierszy; k++){
            if (k >= plansza->liczba_wierszy || plansza->ryby[k][j] == 0){
                break;
            }
            if ((k + 1) >= plansza->liczba_wierszy || plansza->ryby[k + 1][j] == 0){
                (*dziura)++;
            }
            if ((k + 1) < plansza->liczba_wierszy && plansza->pingwiny_na_polu[k + 1][j] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((k - 1) < 0 || plansza->ryby[k - 1][j] == 0){
                (*dziura)++;
            }

            if ((j + 1) >= plansza->liczba_kolumn || plansza->ryby[k][j + 1] == 0){
                (*dziura)++;
            }
            if ((j + 1) < plansza->liczba_kolumn && plansza->pingwiny_na_polu[k][j + 1] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((j - 1) < 0 || plansza->ryby[k][j - 1] == 0){
                (*dziura)++;
            }
            if ((j - 1) >= 0 && plansza->pingwiny_na_polu[k][j - 1] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if (*dziura <= max_dziury && plansza->ryby[k][y] == ryby){
                plansza->pingwiny_na_polu[i][j] = 0;
                int punkty;
                punkty = plansza->ryby[k][y];
                plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
                plansza->ryby[k][y] = 0;
                plansza->pingwiny_na_polu[k][y] = plansza->twoj_nr;
                *dziura = -1;
                break;
            }
            (*dziura) = 0;
        }
    }
}

void znajdz_pole_gora(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc){
    if ((i - 1) >= 0 && plansza->ryby[i - 1][j] != 0){
        int x = i - 1;
        int y = j;
        for (int k = x; k > -1; k--){
            if (k < 0 || plansza->ryby[k][j] == 0){
                break;
            }
            if ((k + 1) >= plansza->liczba_wierszy || plansza->ryby[k + 1][j] == 0){
                (*dziura)++;
            }

            if ((k - 1) < 0 || plansza->ryby[k - 1][j] == 0){
                (*dziura)++;
            }
            if ((k - 1) >= 0 && plansza->pingwiny_na_polu[k - 1][j] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((j + 1) >= plansza->liczba_kolumn || plansza->ryby[k][j + 1] == 0){
                (*dziura)++;
            }
            if ((j + 1) < plansza->liczba_kolumn && plansza->pingwiny_na_polu[k][j + 1] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((j - 1) < 0 || plansza->ryby[k][j - 1] == 0){
                (*dziura)++;
            }
            if ((j - 1) >= 0 && plansza->pingwiny_na_polu[k][j - 1] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if (*dziura <= max_dziury && plansza->ryby[k][y] == ryby){
                plansza->pingwiny_na_polu[i][j] = 0;
                int punkty;
                punkty = plansza->ryby[k][y];
                plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
                plansza->ryby[k][y] = 0;
                plansza->pingwiny_na_polu[k][y] = plansza->twoj_nr;
                *dziura = -1;
                break;
            }
            (*dziura) = 0;
        }
    }
}

void znajdz_pole_prawo(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc){
    if ((j + 1) < plansza->liczba_kolumn && plansza->ryby[i][j + 1] != 0){
        int x = i;
        int y = j + 1;
        for (int k = y; k < plansza->liczba_kolumn; k++){
            if (x == 2 && k == 6 && ryby == 1 && max_dziury == 1){
                printf("LOL\n");
            }
            if (k >= plansza->liczba_kolumn || plansza->ryby[i][k] == 0){
                break;
            }
            if ((k + 1) >= plansza->liczba_kolumn || plansza->ryby[i][k + 1] == 0){
                (*dziura)++;
            }
            if ((k + 1) < plansza->liczba_kolumn && plansza->pingwiny_na_polu[i][k + 1] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((k - 1) < 0 || plansza->ryby[i][k - 1] == 0){
                (*dziura)++;
            }

            if ((i + 1) >= plansza->liczba_wierszy || plansza->ryby[i + 1][k] == 0){
                (*dziura)++;
            }
            if ((i + 1) < plansza->liczba_wierszy && plansza->pingwiny_na_polu[i + 1][k] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((i - 1) < 0 || plansza->ryby[i - 1][k] == 0){
                (*dziura)++;
            }
            if ((i - 1) >= 0 && plansza->pingwiny_na_polu[i - 1][k] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if (*dziura <= max_dziury && plansza->ryby[x][k] == ryby){
                plansza->pingwiny_na_polu[i][j] = 0;
                int punkty;
                punkty = plansza->ryby[x][k];
                plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
                plansza->ryby[x][k] = 0;
                plansza->pingwiny_na_polu[x][k] = plansza->twoj_nr;
                *dziura = -1;
                break;
            }
            (*dziura) = 0;
        }
    }
}

void znajdz_pole_lewo(struct struktura_planszy *plansza, int i, int j, int *dziura, int ryby, int max_dziury, int ostatecznosc){
    if ((j - 1) >= 0 && plansza->ryby[i][j - 1] != 0){
        int x = i;
        int y = j - 1;
        for (int k = y; k > -1; k--){
            if (k < 0 || plansza->ryby[i][k] == 0){
                break;
            }
            if ((k + 1) >= plansza->liczba_kolumn || plansza->ryby[i][k + 1] == 0){
                (*dziura)++;
            }

            if ((k - 1) < 0 || plansza->ryby[i][k - 1] == 0){
                (*dziura)++;
            }
            if ((k - 1) >= 0 && plansza->pingwiny_na_polu[i][k - 1] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((i + 1) >= plansza->liczba_wierszy || plansza->ryby[i + 1][k] == 0){
                (*dziura)++;
            }
            if ((i + 1) < plansza->liczba_wierszy && plansza->pingwiny_na_polu[i + 1][k] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if ((i - 1) < 0 || plansza->ryby[i - 1][k] == 0){
                (*dziura)++;
            }
            if ((i - 1) >= 0 && plansza->pingwiny_na_polu[i - 1][k] == plansza->twoj_nr && ostatecznosc == 0){
                (*dziura) = 0;
                continue;
            }
            if (*dziura <= max_dziury && plansza->ryby[x][k] == ryby){
                plansza->pingwiny_na_polu[i][j] = 0;
                int punkty;
                punkty = plansza->ryby[x][k];
                plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
                plansza->ryby[x][k] = 0;
                plansza->pingwiny_na_polu[x][k] = plansza->twoj_nr;
                *dziura = -1;
                break;
            }
            (*dziura) = 0;
        }
    }
}

void bubbleSort(struct odleglosc_od_krawedzi priorytet_kierunkow[4], int n){
    int i, j;
    struct odleglosc_od_krawedzi temp;
    for (i = 0; i < n; i++){
        for (j = 0; j < n - i - 1; j++){
            if (priorytet_kierunkow[j].odleglosc < priorytet_kierunkow[j + 1].odleglosc){
                temp = priorytet_kierunkow[j];
                priorytet_kierunkow[j] = priorytet_kierunkow[j + 1];
                priorytet_kierunkow[j + 1] = temp;
            }
        }
    }
}
#endif
