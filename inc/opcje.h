#ifndef OPCJE_H
#define OPCJE_H

#include "str_obr2.h"

#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define B_BRAKFILTRU -5
#define B_NIEPOPRAWNYPLIK -6

#define WYM_FILTR 3  /*Rozmiar filtrow*/
extern int czyZgodnie;

int przetwarzaj_opcje(int, char **, t_opcje *);
void wyzeruj_opcje(t_opcje * );

#endif