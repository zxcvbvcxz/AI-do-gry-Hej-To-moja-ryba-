
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "struktury.h"
#include "zapisanie_i_wczytanie.h"
#include "rozstawianie.h"
#include "ruch.h"

int main(int argc, char** argv){
    while(1){
    int rezultat = 0; //zmienna zwracana do systemu, informuje o ewentualnym błędzie

    struct struktura_argumentow_wiersza_polecen argumenty;

    if (strcmp(argv[1], "phase=placement") == 0){// Zapisywanie argumentów wiersza poleceń do struktury jeśli program został uruchomiony w trybie placement
        if (argc != 5){//Sprawdzanie czy ilosc argumentow dla fazy placement jest odpowiednia
            printf("Blad\n");
            printf("Nieprawidlowa ilosc argumentow wejsciowych\n");
            rezultat = 3;
            return (rezultat);
        }
        strncpy(argumenty.ozn_fazy, argv[1], 51);
        if (strncmp(argv[2], "penguins=", 9) == 0){//sprawdzanie poprawnosci skladni drugiego argumentu
            if (isdigit(argv[2][9])){//sprawdzanie czy po frazie penguins= stoi liczba
                if (atof(argv[2] + 9)-(int) (atof(argv[2] + 9)) != 0){//sprawdzanie czy jest to liczba calkowita
                    printf("Blad\n");
                    printf("Ilosc pingwinow musi byc liczba calkowita wieksza od zera\n");
                    rezultat = 3;
                    return (rezultat);
                }
                if (atoi(argv[2] + 9) <= 0){//sprawdzanie czy jest to liczba większa od zera
                    printf("Blad\n");
                    printf("Ilosc pinbwionow musi byc wieksza od 0\n");
                    rezultat = 3;
                    return (rezultat);
                }

                argumenty.pingwiny_max = atoi(argv[2] + 9);
            } else{
                printf("Blad\n");
                printf("Nieprawidlowa wartosc argumentu argumentu \"penguins=\"");
                rezultat = 3;
                return (rezultat);
            }
        } else{
            printf("Blad\n");
            printf("Nieprawidlowy 2 argument wejsciowy\n");
            rezultat = 3;
            return (rezultat);
        }
        strncpy(argumenty.nazwa_pliku1, argv[3], 51);
        strncpy(argumenty.nazwa_pliku2, argv[4], 51);
    } else if (strcmp(argv[1], "phase=movement") == 0){// Zapisywanie argumentów wiersza poleceń do struktury jeśli program został uruchomiony w trybie movement
        if (argc != 4){//Sprawdzanie czy ilosc argumentow dla fazy movement jest odpowiednia
            printf("Blad\n");
            printf("Nieprawidlowa ilosc argumentow wejsciowych\n");
            rezultat = 3;
            return (rezultat);
        }
        strncpy(argumenty.ozn_fazy, argv[1], 51);
        strncpy(argumenty.nazwa_pliku1, argv[2], 51);
        strncpy(argumenty.nazwa_pliku2, argv[3], 51);
    } else{
        printf("Blad\n");
        printf("Nie rozpoznano fazy programu\n");
        printf("Upewnij sie ze argumenty wejsciowe sa poprawne\n");
        rezultat = 3;
        return (rezultat);
    }


    struct struktura_planszy plansza;

    char identyfikator_zaszyty[51] = "Kuboteusz"; //Zaszyty  w programie identyfikator

    wczytaj_rozmiar(&plansza, &argumenty, &rezultat);
    if (rezultat != 0){
        return (rezultat);
    }

    //dynamicznie przydzielam pamięć plansza.ryby
    plansza.ryby = (int**) malloc(plansza.liczba_wierszy * sizeof (int *));
    for (int i = 0; i < plansza.liczba_wierszy; i++){
        plansza.ryby[i] = (int*) malloc(plansza.liczba_kolumn * sizeof (int));
    }
    //dynamicznie przydzielam pamięć plansza.pingwiny_na_polu 
    plansza.pingwiny_na_polu = (int**) malloc(plansza.liczba_wierszy * sizeof (int *));
    for (int i = 0; i < plansza.liczba_wierszy; i++){
        plansza.pingwiny_na_polu[i] = (int*) malloc(plansza.liczba_kolumn * sizeof (int));
    }

    wczytaj(&plansza, &argumenty, &rezultat);//wczytywanie planszy
    if (rezultat != 0){
        return (rezultat);
    }

    plansza.twoj_nr = -1; //twoj numer gracza
    plansza.twoj_numer_w_tablicy = -1;
    for (int i = 0; i < 9; i++){
        if (strncmp(plansza.identyfikator[i], identyfikator_zaszyty, 51) == 0){
            plansza.twoj_nr = plansza.numer_gracza[i];
            plansza.twoj_numer_w_tablicy = i;
        } else if (plansza.twoj_nr == -1 && i == 8){
            for (int j = 0; j < 9; j++){
                if (strncmp(plansza.identyfikator[j], "#Nielegalny_identyfikator#@&$(%&#%@", 51) == 0){
                    strncpy(plansza.identyfikator[j], identyfikator_zaszyty, 51);
                    plansza.numer_gracza[j] = j + 1;
                    plansza.wynik[j] = 0;
                    plansza.twoj_nr = j + 1;
                    plansza.twoj_numer_w_tablicy = j;
                    break;
                }
            }
        }
    }
    plansza.liczba_twoich_pingwinow_na_planszy = 0;


    for (int i = 0; i < plansza.liczba_wierszy; i++){
        for (int j = 0; j < plansza.liczba_kolumn; j++){
            if (plansza.pingwiny_na_polu[i][j] == plansza.numer_gracza[plansza.twoj_numer_w_tablicy]){//liczenie pingwinow na planszy
                plansza.liczba_twoich_pingwinow_na_planszy += 1;
            }
        }
    }
    if (strncmp(argumenty.ozn_fazy, "phase=placement", 51) == 0){//rozkladanie pingwina
        rozstaw(&plansza, &argumenty, &rezultat);
    }
    if (rezultat != 0){
        return (rezultat);
    }
    
    if (strncmp(argumenty.ozn_fazy, "phase=movement", 51) == 0){//poruszanie pingwinem
        rusz(&plansza, &rezultat);
    }
    if (rezultat != 0){
        return (rezultat);
    }
    
    zapisz(&plansza, &argumenty, &rezultat);//zapisywanie planszy
    if (rezultat != 0){
        return (rezultat);
    } 

    return (0);
    }
}

