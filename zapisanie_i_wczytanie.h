
#ifndef ZAPISANIE_I_WCZYTANIE_H
#define ZAPISANIE_I_WCZYTANIE_H
void wczytaj_rozmiar(struct struktura_planszy *s, struct struktura_argumentow_wiersza_polecen *p, int *rezultat); //wczytuje rozmiar planszy do struktury *s
void wczytaj(struct struktura_planszy *s, struct struktura_argumentow_wiersza_polecen *p, int *rezultat); //wczytuje wszystkie dane z pliku zapisu do struktury *s
void zapisz(struct struktura_planszy *s, struct struktura_argumentow_wiersza_polecen *p, int *rezultat); //zapisuje dane do pliku
void rysuj_plansze(struct struktura_planszy plansza);//rysuje plansze
#endif

