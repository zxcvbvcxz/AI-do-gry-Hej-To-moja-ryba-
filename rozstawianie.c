

#include "struktury.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "zapisanie_i_wczytanie.h"

#define AI
#ifndef AI

int rozstaw(struct struktura_planszy *plansza, struct struktura_argumentow_wiersza_polecen *argumenty, int *rezultat){
    while (1){
        if (plansza->liczba_twoich_pingwinow_na_planszy == argumenty->pingwiny_max){//jeśli zostaly juz rozstawione wszystkie pingwiny to konczy dzialanie funkcji
            *rezultat = 1;
            break;
        } else if (plansza->liczba_twoich_pingwinow_na_planszy >= argumenty->pingwiny_max){
            *rezultat = 2;
            printf("Blad\n");
            printf("W zapisanym pliku jest za dużo twoich pingwinow na polu\n");
            *rezultat = 2;
            break;
        }
        int czy_mozliwy_ruch = 0; //zmienia wartosc w jesli jest mozliwy ruch do wykoanania 
        for (int i = 0; i < plansza->liczba_wierszy; i++){//sprawdzanie czy jest na planszy pole z 1 ryba
            for (int j = 0; j < plansza->liczba_kolumn; j++){
                if (plansza->ryby[i][j] == 1){//istnieje pole z 1 ryba
                    czy_mozliwy_ruch = 1; //zmiana wartosci
                    goto ucieczka; //wyjscie z petli po znalezieniu pola z rybami
                }
            }
        }
ucieczka:
        ;
        if (czy_mozliwy_ruch == 0){//jesli ruch jest niemozliwy rezultat jest zmieniany na 2 i funkcja konczy dzialanie
            printf("Blad\n");
            printf("Na odczytanej planszy nie ma miejsca na rozstawienie pingwina\n");
            *rezultat = 2;
            break;
        }
        rysuj_plansze(*plansza);
        printf("Podaj wspolrzedne pola na ktorym chcesz umiescic pingwina\n");
        int x, y;
        scanf("%d %d", &y, &x);//zczytuje najpier numer kolumny, pozniej numer wiersza
        if ((x - 1) < 0 || (x - 1)>(plansza->liczba_wierszy - 1) || (y - 1) < 0 || (y - 1)>(plansza->liczba_kolumn - 1)){
            printf("Podane współrzędne są poza zakresem planszy\n");
            continue;
        }
        if (plansza->ryby[x - 1][y - 1] != 1){
            printf("Nie mozesz umiescic pingwina na tym polu\n");
            continue;
        }
        plansza->pingwiny_na_polu[x - 1][y - 1] = plansza->numer_gracza[plansza->twoj_numer_w_tablicy];
        int punkty;
        punkty = plansza->ryby[x - 1][y - 1];
        plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
        plansza->ryby[x - 1][y - 1] = 0;
        break;

    }
    return (0);
}



#else

int rozstaw(struct struktura_planszy *plansza, struct struktura_argumentow_wiersza_polecen *argumenty, int *rezultat){
    while (1){
        if (plansza->liczba_twoich_pingwinow_na_planszy == argumenty->pingwiny_max){//jeśli zostaly juz rozstawione wszystkie pingwiny to konczy dzialanie funkcji
            *rezultat = 1;
            break;
        } else if (plansza->liczba_twoich_pingwinow_na_planszy >= argumenty->pingwiny_max){
            *rezultat = 2;
            printf("Blad\n");
            printf("W zapisanym pliku jest za dużo twoich pingwinow na polu\n");
            *rezultat = 2;
            break;
        }
        int czy_mozliwy_ruch = 0; //zmienia wartosc jesli jest mozliwy ruch do wykonania 
        for (int i = 0; i < plansza->liczba_wierszy; i++){//sprawdzanie czy jest na planszy pole z 1 ryba
            for (int j = 0; j < plansza->liczba_kolumn; j++){
                if (plansza->ryby[i][j] == 1){
                    czy_mozliwy_ruch = 1; //zmiana wartosci
                    goto ucieczka; //wyjscie z petli po znalezieniu pola z rybami
                }
            }
        }
ucieczka:
        ;
        if (czy_mozliwy_ruch == 0){//jesli ruch jest niemozliwy rezultat jest zmieniany na 2 i funkcja konczy dzialanie
            printf("Blad\n");
            printf("Na odczytanej planszy nie ma miejsca na rozstawienie pingwina\n");
            *rezultat = 2;
            break;
        }

        int dziura = 0;//oznacza ilosc pol blokujacych ruch pingwina
        int x, y;
        for (int i = 0; i < plansza->liczba_wierszy; i++){//Sprawdzanie czy istnieje pole na którym stoi przeciwnik z tylko jednym kierunkiem ucieczki
            for (int j = 0; j < plansza->liczba_kolumn; j++){//jeśli istnieje to program stawia pingwina tak by zablokować przeciwnika, jesli jest to mozliwe
                if (plansza->pingwiny_na_polu[i][j] != 0 && plansza->pingwiny_na_polu[i][j] != plansza->twoj_nr){
                    if ((i + 1) >= plansza->liczba_wierszy || plansza->ryby[i + 1][j] == 0){
                        dziura++;
                    } else{
                        x = i + 1;
                        y = j;
                    }
                    if ((i - 1) < 0 || plansza->ryby[i - 1][j] == 0){
                        dziura++;
                    } else{
                        x = i - 1;
                        y = j;
                    }
                    if ((j + 1) >= plansza->liczba_kolumn || plansza->ryby[i][j + 1] == 0){
                        dziura++;
                    } else{
                        x = i;
                        y = j + 1;
                    }

                    if ((j - 1) < 0 || plansza->ryby[i][j - 1] == 0){
                        dziura++;
                    } else{
                        x = i;
                        y = j - 1;
                    }
                    if (dziura == 3 && plansza->ryby[x][y] == 1){
                        plansza->pingwiny_na_polu[x][y] = plansza->numer_gracza[plansza->twoj_numer_w_tablicy];
                        int punkty;
                        punkty = plansza->ryby[x][y];
                        plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
                        plansza->ryby[x][y] = 0;
                        goto koniec;
                    }

                }
            }
        }

        dziura = 0;
        int max_dziury = 0;//oznacza maksymalna ilosc pol blokujacych ruch pingwina
        for (int i = 0; i < plansza->liczba_wierszy; i++){//szukanie pola z najwyzsza liczba mozliwych kierunkow ruchu
            for (int j = 0; j < plansza->liczba_kolumn; j++){//jeśli znajdzie takie pole to stawia pingwina               
                if (plansza->ryby[i][j] == 1){
                    if ((i + 1) >= plansza->liczba_wierszy || plansza->ryby[i + 1][j] == 0){
                        dziura++;
                    }
                    if ((i - 1) < 0 || plansza->ryby[i - 1][j] == 0){
                        dziura++;
                    }
                    if ((j + 1) >= plansza->liczba_kolumn || plansza->ryby[i][j + 1] == 0){
                        dziura++;
                    }
                    if ((j - 1) < 0 || plansza->ryby[i][j - 1] == 0){
                        dziura++;
                    }
                    if (dziura <= max_dziury){
                        plansza->pingwiny_na_polu[i][j] = plansza->numer_gracza[plansza->twoj_numer_w_tablicy];
                        int punkty;
                        punkty = plansza->ryby[i][j];
                        plansza->wynik[plansza->twoj_numer_w_tablicy] += punkty;
                        plansza->ryby[i][j] = 0;
                        goto koniec;
                    }
                    dziura = 0;
                }
                if ((i == (plansza->liczba_wierszy - 1)) && (j == (plansza->liczba_kolumn - 1))){//jesli na planszy nie ma pola spelniajacego wymagania to obniza je i szuka ponownie
                    max_dziury++;
                    i = 0;
                    j = -1; //bo 1 zostanie dodane napoczątku pętli
                }
            }
        }

        break;
    }
koniec:
    return (0);
}
#endif
