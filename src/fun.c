#include<stdio.h>
#include <math.h>
#include<stdlib.h>
#include "../inc/fun.h"
#include "../inc/str_obr2.h"

#define WYM_FILTR 3
/************************************************************************************
 * Funkcja przeprowadza operację negatywu      	       	       	       	    *
 *										    *
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie	
 * \param[in] int tab[3][3] - tablica 3x3 posiadajace odpowiednia wartosci dla danego typu filtra	    
 * \param[out] *tablica ze zmienionym obrazem po operacji splotu	    *
 ************************************************************************************/
void splot(t_obraz *wsk, int tab[WYM_FILTR][WYM_FILTR]){

  
  int (*temp_piksele)[wsk->wym_x];                  /*To jest wskaznik na oryginalna*/
  temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele;/*tablice dwuwymiarowa dynamiczna*/

  for(int i=1;i < wsk->wym_y -1 ;i++){
    for(int j=1;j<wsk->wym_x -1;j++){

        temp_piksele[i][j] = tab[0][0]*temp_piksele[i-1][j-1] + tab[0][1]*temp_piksele[i-1][j] + tab[0][2]*temp_piksele[i-1][j+1] + tab[1][0]*temp_piksele[i][j-1] + tab[1][1]*temp_piksele[i][j] + tab[1][2]*temp_piksele[i][j+1]+ tab[2][0]*temp_piksele[i+1][j-1]+ tab[2][1]*temp_piksele[i+1][j]+ tab[2][2]*temp_piksele[i+1][j+1];
        if(temp_piksele[i][j]>1)temp_piksele[i][j]=temp_piksele[i][j]/9;
        if(temp_piksele[i][j]<0)temp_piksele[i][j] = 0;
        if(temp_piksele[i][j]>wsk->odcieni)temp_piksele[i][j]=wsk->odcieni;
    }
  } 
}

/************************************************************************************
 * Funkcja przeprowadza konwersje z obrazu PPM na PGM     	       	       	       	*
 *										    *
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie	    
 * \param[out] *typ_obrazu w sktrukturze zamienia sie na wartosc 4 odpowiadajacej
 * konwersji z PPM na PGM
 * \param[out] *tablica ze skonwertowanym obrazem	 *
 ************************************************************************************/
void konwersja(t_obraz *wsk){
  
  int g=0,skala_szarosci=-100; /* g - pomocnicza zmienna */
                              /*skala_szarosci - zmienna do zapisu maksymalnej granicy szarosci*/
  int (*temp_piksele)[wsk->wym_x];                  /*To jest wskaznik na oryginalna tablice dwuwymiarowa dynamiczna*/
  temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele;

  for(int i=0;i < wsk->wym_y;i++){
    for(int j=0;j<wsk->wym_x;j+=3){
    temp_piksele[i][g]=(temp_piksele[i][j]+temp_piksele[i][j+1]+temp_piksele[i][j+2])/3;
    if(temp_piksele[i][g] > skala_szarosci) skala_szarosci=temp_piksele[i][g];
    g++;}
    g=0;
  }
  wsk->odcieni = skala_szarosci; /*Zmiana cyfry oznaczającej górny margines szarosci*/
  wsk->typ_obr = 4; /*Typ obrazu po konwersji z PPM na PGM*/
}

/************************************************************************************
 * Funkcja przeprowadza operację negatywu      	       	       	       	    *
 *										    *
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie	    
 * \param[out] *tablica ze zmienionym obrazem po operacji negatyw		    *
 ************************************************************************************/
void negatyw(t_obraz *wsk){
    
    int (*temp_piksele)[wsk->wym_x];                    /* Tworzenie pomocnego wskaźnika do*/
    temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele;  /* wykonywania operacji na obrazie */

  for(int i=0;i<wsk->wym_y;i++)
    for(int j=0;j<wsk->wym_x;j++)
     temp_piksele[i][j] = (wsk->odcieni)- (temp_piksele[i][j]);
}

/************************************************************************************
 * Funkcja przeprowadza operację półprogowania bielii      	       	       	       	    *
 *		
 * \param[in]	prog wartosc progu (z przedzialu 0.0 do 1.0) 							    *
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie		    		    *
 * \param[out] tablica ze zmienionym obrazem po operacji polprogowania bieli		    *
 ************************************************************************************/
void progbiel(float prog,t_obraz *wsk){
  int pprog = prog * (wsk->odcieni);
  
  int (*temp_piksele)[wsk->wym_x];                    /* Tworzenie pomocnego wskaźnika do*/
  temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele;  /* wykonywania operacji na obrazie */
  
  for(int i=0;i<wsk->wym_y;i++)
    for(int j=0;j<wsk->wym_x;j++){
      if(temp_piksele[i][j]> pprog)
        temp_piksele[i][j] = wsk->odcieni;}
}

/************************************************************************************
 * Funkcja przeprowadza operację konturowania     	       	       	       	    *
 *		
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie						    *					    *
 * \param[out]  tablica ze zmienionym obrazem po operacji konturowania		    *
 ************************************************************************************/
void konturowanie(t_obraz *wsk){
   
  int (*temp_piksele)[wsk->wym_x];                      /* Tworzenie pomocnego wskaźnika do*/
  temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele;    /* wykonywania operacji na obrazie */
  
  int a, b;
  for(int i = 0;i < wsk->wym_y; ++i)
    for(int j = 0; j < wsk->wym_x; ++j)
    {
      a = temp_piksele[i+1][j] - temp_piksele[i][j];
      b = temp_piksele[i][j+1] - temp_piksele[i][j];
      if(i == 0 && j < wsk->wym_x)   /* warunek dla pierwszej linijki*/
        temp_piksele[i][j] = abs(b);
      if(i != 0 && j < wsk->wym_x) 
        temp_piksele[i][j] = abs(b) + abs(a);
      if(j == ((wsk->wym_x)-1)) /* warunek dla pikseli skrajnych z prawej strony */
        temp_piksele[i][j] = abs(a);
      if(i == ((wsk->wym_y)-1)) /* warunek dla ostatniej linijki */
        temp_piksele[i][j] = abs(b);
      if((i==wsk->wym_y-1) && (j == (wsk->wym_x)-1))/* warunek dla skrajnego prawego rogu */
        temp_piksele[i][j] = abs(b) - abs(a);
    }
}