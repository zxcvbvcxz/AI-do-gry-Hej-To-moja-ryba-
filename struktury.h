#ifndef STRUKTURY_H
#define STRUKTURY_H

struct struktura_planszy {//Struktura przechowuje wszystkie dane jakie będą potrzebne przez program i mogą być odczytane z pliku zapisu
    int liczba_wierszy; //Przechowuje informacje o ilości wierszy z których składa się plansza
    int liczba_kolumn; //Przechowuje informacje o ilości kolumn z których składa się plansza
    int **ryby; //Przechouwje informacje o ilości ryb na każdym polu
    int **pingwiny_na_polu; //Przechouwje informacje o tym który gracz stoi na danym polu lub czy jest ono puste
    int liczba_twoich_pingwinow_na_planszy; //Przechowuje informacje o tym ile pingwinów gracza znajduje się w tej chwili na planszy
    int wynik[9]; //Przechowuje inforamcje o wyniku gracza
    char identyfikator[9][51]; //identyfikator gracza
    int numer_gracza[9]; //Przechowuje informacje o numerze gracza
    int twoj_nr; //twoj numer gracza
    int twoj_numer_w_tablicy;//twoj numer w tablicy
};

struct struktura_argumentow_wiersza_polecen {//Struktura przechowuje dane przekazywane programowi przez argumenty wiersza poleceń 
    int pingwiny_max; //Przechowuje informacje o liczbie pingwinów, które muszą zostać rozstawione na planszy
    char ozn_fazy[51]; //Przechowuje informacje o fazie w której został uruchomiony program
    char nazwa_pliku1[51]; //Przechowuje informacje o nazwie pliku zapisu z którego będą odczytywane dane
    char nazwa_pliku2[51]; //Przechowuje informacje o nazwie pliku zapisu do którego będą zapisywane dane
};

#endif
