#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struktury.h"
#include "zapisanie_i_wczytanie.h"
#include <ctype.h>

void wczytaj_rozmiar(struct struktura_planszy *s, struct struktura_argumentow_wiersza_polecen *p, int *rezultat){
    while (1){
        FILE *mojplik; //deklaruje wskaznik na zmienna FILE
        mojplik = fopen(p->nazwa_pliku1, "r"); //otwieram plik z zamiarem wczytania 
        if (mojplik == NULL){
            printf("Blad\n");
            printf("Nie udało się otworzyć pliku do odczytu\n");
            *rezultat = 2;
            break;
        }

        if (fscanf(mojplik, "%d", &s->liczba_wierszy) == EOF){ //pobieranie liczby wierszy z pliku
            printf("Blad\n");
            printf("Nie udało sie odczytac liczby wierszy\n");
            *rezultat = 2;
            break;
        }

        if (fscanf(mojplik, "%d", &s->liczba_kolumn) == EOF){ //pobieranie liczby kolumn z pliku
            printf("Blad\n");
            printf("Nie udało sie odczytac liczby kolumn\n");
            *rezultat = 2;
            break;
        }

        if (s->liczba_wierszy <= 0 || s->liczba_kolumn <= 0){
            printf("Blad\n");
            printf("Plik odczytu zawiera nieprawidlowa informacje o rozmiarze planszy\n");
            *rezultat = 2;
            break;
        }

        if (fclose(mojplik) != 0){
            printf("Blad");
            printf("Nie udało się zamknąć pliku do odczytu\n");
            *rezultat = 3;
            break;
        }
        break;
    }

}

void wczytaj(struct struktura_planszy *s, struct struktura_argumentow_wiersza_polecen *p, int *rezultat){
    while (1){
        FILE *mojplik; //deklaruje wskaznik na zmienna FILE
        mojplik = fopen(p->nazwa_pliku1, "r"); //otwieram plik z zamiarem wczytania
        if (mojplik == NULL){
            printf("Blad\n");
            printf("Nie udało się otworzyć pliku do odczytu\n");
            *rezultat = 2;
        }

        fscanf(mojplik, "%d%d", &s->liczba_wierszy, &s->liczba_kolumn); //pobieram liczbe wierszy i kolumn

        int i, j;
        for (i = 0; i < s->liczba_wierszy; i++){//pobieram pola planszy
            for (j = 0; j < s->liczba_kolumn; j++){
                int pole;
                char pole_ch[51];
                if (fscanf(mojplik, "%s", &pole_ch) == EOF){
                    ; //pobieram dane z pola
                    printf("Blad\n");
                    printf("W odczytanym pliku jest za mało pol planszy\n");
                    *rezultat = 2;
                    goto koniec; //idzie na koniec funkcji
                }
                if (isalpha(pole_ch[0]) != 0){
                    printf("Blad\n");
                    printf("W odczytanym pliku jest za mało pol planszy\n");
                    *rezultat = 2;
                    goto koniec; //idzie na koniec funkcji
                } else{
                    pole = atoi(pole_ch);
                }//TUTAJ ZMIANY PO RAPORCIE 5
                switch (pole){//na podstawie odczytanej liczby z pola uzupełniam strukturę
                    case 00: s->ryby[i][j] = 0;
                        s->pingwiny_na_polu[i][j] = 0;
                        break;
                    case 10: s->ryby[i][j] = 1;
                        s->pingwiny_na_polu[i][j] = 0;
                        break;
                    case 20: s->ryby[i][j] = 2;
                        s->pingwiny_na_polu[i][j] = 0;
                        break;
                    case 30: s->ryby[i][j] = 3;
                        s->pingwiny_na_polu[i][j] = 0;
                        break;
                    default: s->ryby[i][j] = 0;
                        s->pingwiny_na_polu[i][j] = pole%10;//TUTAJ ZMIANY PO RAPORCIE 5
                        break;

                }
            }
        }

        for (i = 0; i < 9; i++){
            strncpy(s->identyfikator[i], "#Nielegalny_identyfikator#@&$(%&#%@", 51);
        }
        char temp_ch[51];
        for (i = 0; i < 9; i++){//pobieram identyfikator, numer gracza i wynik z pliku   
            if (fscanf(mojplik, "%s", &(temp_ch)) == EOF){
                continue;
            }
            strncpy(s->identyfikator[i], temp_ch, 51);

            if (fscanf(mojplik, "%s", &(temp_ch)) == EOF){
                printf("Blad\n");
                printf("Program nie mogl odczytac numeru gracza z pliku z zapisem\n");
                *rezultat = 2;
                goto koniec; //idzie na koniec funkcji
            } else if (isalpha(temp_ch[0]) != 0){
                printf("Blad\n");
                printf("Program nie mogl odczytac numeru gracza z pliku z zapisem\n");
                *rezultat = 2;
                goto koniec; //idzie na koniec funkcji
            } else if (atoi(temp_ch) <= 0 || atoi(temp_ch) >= 9 || (atof(temp_ch) - atoi(temp_ch)) != 0){
                printf("Blad\n");
                printf("Numer gracza w pliku z zapisem ma niedozwolona wartosc\n");
                *rezultat = 2;
                goto koniec; //idzie na koniec funkcji
            } else{
                s->numer_gracza[i] = atoi(temp_ch);
            }
            if (fscanf(mojplik, "%s", &(temp_ch)) == EOF){
                printf("Blad\n");
                printf("Program nie mogl odczytac wyniku gracza z pliku z zapisem\n");
                *rezultat = 2;
                goto koniec; //idzie na koniec funkcji
            } else if (isalpha(temp_ch[0]) != 0){
                printf("Blad\n");
                printf("Program nie mogl odczytac wyniku gracza z pliku z zapisem\n");
                *rezultat = 2;
                goto koniec; //idzie na koniec funkcji
            } else if ((atof(temp_ch) - atoi(temp_ch)) != 0 || (atoi(temp_ch) < 0)){
                printf("Blad\n");
                printf("Wynik w pliku z zapisem ma niedozwolona wartosc\n");
                *rezultat = 2;
                goto koniec; //idzie na koniec funkcji
            } else{
                s->wynik[i] = atoi(temp_ch);
            }

        }

        if (fclose(mojplik) != 0){//zamykam plik
            printf("Blad\n");
            printf("Blad przy zamykaniu pliku\n");
            *rezultat = 3;
            break;
        }
koniec:
        break;
    }
}

void zapisz(struct struktura_planszy *s, struct struktura_argumentow_wiersza_polecen *p, int *rezultat){
    while (1){
        FILE *mojplik; //deklaruje wskaznik na zmienną FILE
        mojplik = fopen(p->nazwa_pliku2, "w"); //otwieram plik board.txt z zamiarem zapisu
        if (mojplik == NULL){
            printf("Blad\n");
            printf("Nie udało się otworzyć pliku do zapisu\n");
            *rezultat = 3;
            break;
        }

        fprintf(mojplik, "%d %d\r\n", s->liczba_wierszy, s->liczba_kolumn); //zapisuje liczbę kolumn i wierszy do pliku
        int i, j;
        for (i = 0; i < s->liczba_wierszy; i++){//zapisuje pola do pliku 
            for (j = 0; j < s->liczba_kolumn; j++){
                fprintf(mojplik, "%d%d ", s->ryby[i][j], s->pingwiny_na_polu[i][j]);
            }
            fprintf(mojplik, "\r\n");
        }

        for (i = 0; i < 9; i++){//zapisuje indetyfikatory, numery gracza i wyniki do pliku
            if (strncmp(s->identyfikator[i], "#Nielegalny_identyfikator#@&$(%&#%@", 1) != 0){
                fprintf(mojplik, "%s %d %d\r\n", s->identyfikator[i], s->numer_gracza[i], s->wynik[i]);
            }
        }

        if (fclose(mojplik) != 0){
            printf("Blad");
            printf("Nie udało się zamknąć pliku do odczytu\n");
            *rezultat = 3;
            break;
        }
        break;
    }
}

void rysuj_plansze(struct struktura_planszy plansza){
    if (plansza.liczba_wierszy < 10){
        printf("  ");
    } else{
        printf("   ");
    }
    for (int k = 0; k < plansza.liczba_kolumn; k++){
        if (k < 9){
            printf("%d  ", k + 1);
        } else{
            printf("%d ", k + 1);
        }
    }
    printf("\n");

    for (int i = 0; i < plansza.liczba_wierszy; i++){
        if (plansza.liczba_wierszy >= 10 && i < 9){
            printf("%d  ", i + 1);
        } else if (plansza.liczba_wierszy >= 10 && i >= 9){
            printf("%d ", i + 1);
        } else{
            printf("%d ", i + 1);
        }
        for (int j = 0; j < plansza.liczba_kolumn; j++){
            printf("%d", plansza.ryby[i][j]);
            printf("%d ", plansza.pingwiny_na_polu[i][j]);
        }
        printf("\n");
    }
}