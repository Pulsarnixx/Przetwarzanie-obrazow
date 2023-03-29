#ifndef OBSLUGA_PGM_H
#define OBSLUGA_PGM_H
#include "str_obr2.h"

extern int czyZgodnie;


int czytaj(FILE *, t_obraz *);
void wyswietl(char *);
void zapisz(FILE *, t_obraz *);


#endif