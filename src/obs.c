#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../inc/fun.h"
#include"../inc/str_obr2.h"
#include "../inc/opcje.h"

#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

extern int czyZgodnie; /* zmienna globalna określająca czy plik PGM lub 
                      PPM jest poprawny i istnieje, jesli nie to ma wartosc 1*/

/************************************************************************************
 * Funkcja wczytuje obraz PGM lub PPM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM lub PPM		    *
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie				    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

int czytaj(FILE *plik_we, t_obraz *wsk) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;
  wsk->typ_obr = 0;

  /*Sprawdzenie czy podano prawid�owy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    czyZgodnie = 1;
    return (0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien by� P2 lub P3*/
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') && (buf[1]!='3') || koniec) {  /* Czy jest magiczne "P2" lub "P3"? */
    fprintf(stderr,"Blad: To nie jest plik PGM lub PPM\n");
    czyZgodnie = 1;
    return (0);
  }
  
  /*Okreslanie w strukturze t_obraz jakiego typu jest obraz*/
  if(buf[1]=='2') wsk->typ_obr = 2; /* typ_obr 2 oznacza PGM (bo P2)*/
  if(buf[1]=='3') wsk->typ_obr = 3; /* typ_obr 3 oznacza PPM (bo P3)*/
                                    /* typ_obr 4 PGM po konwersji z PPM*/
  
  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",&wsk->wym_x,&wsk->wym_y,&wsk->odcieni)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    czyZgodnie = 1;
    return(0);
  }
  /*Tworzenie dwuwymiarowej tablicy dynamicznej */

  if(wsk->typ_obr == 3) wsk->wym_x=wsk->wym_x*3; /*Jesli obraz jest PPM to wym_x 3 razy większe ze względu na barwy*/

  wsk->piksele = malloc(wsk->wym_x*wsk->wym_y*sizeof(int));   /* Rezerwacja miejsca w pamięci na tablicę dwuwymiarową o wymiarach */
                                                              /* wym_x * wym_y, uwzględniając miejsce na zmienne typu int*/

  int (*temp_piksele)[wsk->wym_x];                    /* Tworzenie pomocnego wskaźnika do*/
  temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele;  /* wykonywania operacji na obrazie */

  /* Pobranie obrazu i zapisanie w tablicy wskazywanej przez void * piksele w strukturze t_obraz*/
  for (i=0;i<wsk->wym_y;i++) {
    for (j=0;j<wsk->wym_x;j++) {
      if (fscanf(plik_we,"%d",&(temp_piksele[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
  czyZgodnie = 1;
	return(0);
      }
    }
  }
  return (wsk->wym_x)*(wsk->wym_y);   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

/* Funkcja wyswietla obraz PGM oraz PPM przy pomocy display */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/************************************************************************************
 * Funkcja zapisuje obraz PGM lub PPM z tablicy do nowo powstałego pliku   	       	       	       	    *
 *										    *
 * \param[in] t_obraz *wsk - wskaźnik na strukturę posiadającą informacje o obrazie					    *
 * \param[out] plik_wy nowo powstały plik z obrazem PGM	lub PPM	    *
 ************************************************************************************/
void zapisz(FILE *plik_wy, t_obraz *wsk){
  
  int (*temp_piksele)[wsk->wym_x];                   /* Tworzenie pomocnego wskaźnika do*/
  temp_piksele = (int (*)[wsk->wym_x]) wsk->piksele; /* wykonywania operacji na obrazie */

 /* Wypisanie do pliku magicznego nuemru, szerokosci, wysokosci i skali szarosci obrazka, ktory chcemy zapisac */
 
  if(wsk->typ_obr==2)  /*Zapis pliku PGM*/
    {
      fprintf(plik_wy,"P2\n");
      fprintf(plik_wy,"%d %d\n%d\n",wsk->wym_x, wsk->wym_y, wsk->odcieni);
      for(int i = 0; i < wsk->wym_y; ++i){
          for(int j = 0; j < (wsk->wym_x); ++j)
          {
            fprintf(plik_wy, "%d ", temp_piksele[i][j]);
          }
          fprintf(plik_wy, "\n");
        }      
      }
  else if(wsk->typ_obr==4) /*Zapis pliku PGM po konwersji z PPM*/
  {
    fprintf(plik_wy,"P2\n");
    fprintf(plik_wy,"%d %d\n%d\n",(wsk->wym_x/3), wsk->wym_y, wsk->odcieni);
    for(int i = 0; i < wsk->wym_y; ++i){
        for(int j = 0; j < (wsk->wym_x/3); ++j)
        {
          fprintf(plik_wy, "%d ", temp_piksele[i][j]);
        }
        fprintf(plik_wy, "\n");
      }
  }
  else if(wsk->typ_obr==3) /*Zapis pliku PPM bez konwersji */
  {
    fprintf(plik_wy,"P3\n");
    fprintf(plik_wy,"%d %d\n%d\n",wsk->wym_x/3, wsk->wym_y, wsk->odcieni);
    for(int i = 0; i < wsk->wym_y; ++i)
      {
        for(int j = 0; j < wsk->wym_x; ++j)
        {
          fprintf(plik_wy, "%d ", temp_piksele[i][j]);
        }
        fprintf(plik_wy, "\n");
      }
  }
}
