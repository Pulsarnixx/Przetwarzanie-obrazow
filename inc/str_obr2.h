#ifndef STRUKTURY_H
#define STRUKTURY_H

/* typ do przechowywania obrazów monochoromatycznych i wielokolorowych*/
typedef struct {
int wym_x, wym_y, odcieni; /* Wymiary obrazu oraz maksymalna wartość składowej szarości / składowych kolorowych obrazu / */
void *piksele;  /*wskaźnik na tablice na której beda wykonywane operacje*/
int typ_obr; /* Typ obrazu: wartosc 2 odpowiada PGM, 3 PPM, zaś 4 odpowiada wartosci obrazu PGM przekonwertowanego z PPM*/
} t_obraz;

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw,progowanie,konturowanie,wyswietlenie,splot;      /* opcje */
  int w_progu;              /* wartosc progu dla opcji progowanie */ 
} t_opcje;
#endif