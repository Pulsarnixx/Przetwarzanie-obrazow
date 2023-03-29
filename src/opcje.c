/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA OPCJE Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2020 KCiR  */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../inc/obs.h"
#include"../inc/fun.h"
#include"../inc/opcje.h"



/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje(t_opcje * wybor) {
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->wyswietlenie=0;
  wybor->splot=0;
}

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d]    */
/*         [-s typ filtru (od s1 do s9)] }                              */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wyb�r, do tego ustawia na 1 pola dla opcji, ktore  */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne            */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor) {
  
  int i, prog, odczytano; 
  char *nazwa_pliku_we = NULL, *nazwa_pliku_wy = NULL;
  char *filtr = NULL; /*zmienna wskazująca na rodzaj filtru w trakcie wywolania programu*/

  /*Rodzaje filtrow, odpowiednio od s1 do s9*/

  int filtrUsred[WYM_FILTR][WYM_FILTR] = {{1,1,1},{1,1,1},{1,1,1}}, filtrUsredWzm[WYM_FILTR][WYM_FILTR] = {{1,1,1},{1,2,1},{1,1,1}}, filtrRobertsa[WYM_FILTR][WYM_FILTR] = {{0,0,0},{-1,0,0},{0,1,0}};
  int filtrPrewitta[WYM_FILTR][WYM_FILTR] = {{-1,-1,-1},{0,0,0},{1,1,1}}, filtrSobela[WYM_FILTR][WYM_FILTR] = {{-1,-2,-1},{0,0,0},{1,2,1}}, filtrNaroznik[WYM_FILTR][WYM_FILTR] = {{1,1,1},{-1,-2,1},{-1,-1,1}};
  int filtrlaps1[WYM_FILTR][WYM_FILTR] = {{0,-1,0},{-1,4,-1},{0,-1,0}},filtrlaps2[WYM_FILTR][WYM_FILTR] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}}, filtrlaps3[WYM_FILTR][WYM_FILTR] = {{1,-2,1},{-2,4,-2},{1,-2,1}};


  t_obraz *wsk;                                   /*Tworzenie wskaźnika na strukture*/
  wsk = (t_obraz *) malloc(sizeof(t_obraz));      /*w której sa informacje o obrazie*/

  wyzeruj_opcje(wybor);
  wybor->plik_wy=stdout;        /* na wypadek gdy nie podano opcji "-o" */

  for (i=1; i<argc; i++) {
    
    if (argv[i][0] != '-') return B_NIEPOPRAWNAOPCJA;   /* blad: to nie jest opcja - brak znaku "-" */  
    switch (argv[i][1]) {
    
    case 'i': {                 /* opcja z nazwa pliku wejsciowego */
      
      if (++i<argc) {  /* wczytujemy kolejny argument jako nazwe pliku */
         
	      nazwa_pliku_we=argv[i];

	    if (strcmp(nazwa_pliku_we,"-")==0) wybor->plik_we=stdin; /* gdy nazwa jest "-" ustawiamy wejscie na stdin*/            
	    else wybor->plik_we=fopen(nazwa_pliku_we,"r"); /* otwieramy wskazany plik   */                        

      } 
      else  return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      break;
    }
    
    case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
      
        if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	        nazwa_pliku_wy=argv[i];
	        if (strcmp(nazwa_pliku_wy,"-")==0)/* gdy nazwa jest "-"         */
	          wybor->plik_wy=stdout;          /* ustawiamy wyjscie na stdout */
	        else                         /* otwieramy wskazany plik    */
	          wybor->plik_wy=fopen(nazwa_pliku_wy,"w");
        }
      else 
      return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      
      break;
    }

    case 'p': {                 /*mamy wykonac polprogowanie bieli*/
      if (++i<argc) { /* wczytujemy kolejny argument jako wartosc progu */
	      if (sscanf(argv[i],"%d",&prog)==1) {
	        wybor->progowanie=1;
	        wybor->w_progu=prog;
	      } else return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
      } 
      else return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
      break;
    }

    case 'n': {                 /* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }

    case 'k': {                 /* mamy wykonac konturowanie */
      wybor->konturowanie=1;
      break;
    }
    
    case's':{                    /*mamy wykonac splot z odpowiednim filtrem*/
      if(++i<argc){
        filtr = argv[i]; 
        wybor->splot = 1; /*jest jakis filtr wiec wykonujemy*/
      }
      break;
    }
    
    case 'd': {                 /* mamy wyswietlic obraz */
      wybor->wyswietlenie=1;
      break;
    }
  
    default:{
      return B_NIEPOPRAWNAOPCJA; /* nierozpoznana opcja */
      break;               
    } 
    
     } /* koniec switch */
  } /* koniec for */

/*Sprawdzanie wszystkich flag i wykonywanie operacji na obrazie*/
/*Jezeli obraz jest typu PPM to przed wykonywaniem funkcji negatywu,*/
/*polprogowania bieli i konturawania zostaje wykonywana konwersja do PGM*/
  if (wybor->plik_we!=NULL){
        odczytano = czytaj(wybor->plik_we, wsk);
        fclose(wybor->plik_we);
        if(czyZgodnie == 0){
        if(wybor->negatyw == 1){
          if(wsk->typ_obr==3) konwersja(wsk);
          negatyw(wsk);
        }

        if(wybor->konturowanie == 1){
          if(wsk->typ_obr==3) konwersja(wsk);  
          konturowanie(wsk);
        }

        if(wybor->progowanie == 1){
          if(wsk->typ_obr==3) konwersja(wsk);
          progbiel(wybor->w_progu, wsk);
        }

        if(wybor->wyswietlenie == 1){
          if(odczytano != 0) {
          }wyswietl(nazwa_pliku_we);
        }

        if(wybor->splot == 1){
              /*Warunki potrzebne do roztrzygniecia za pomoca ktorego filtra wykonac splot*/
              if(strcmp(filtr,"s1")==0) splot(wsk,filtrUsred);
              else if(strcmp(filtr,"s2")==0) splot(wsk,filtrUsredWzm);
              else if(strcmp(filtr,"s3")==0) splot(wsk,filtrRobertsa);
              else if(strcmp(filtr,"s4")==0) splot(wsk,filtrPrewitta);
              else if(strcmp(filtr,"s5")==0) splot(wsk,filtrSobela);
              else if(strcmp(filtr,"s6")==0) splot(wsk,filtrNaroznik);
              else if(strcmp(filtr,"s7")==0) splot(wsk,filtrlaps1);
              else if(strcmp(filtr,"s8")==0) splot(wsk,filtrlaps2);
              else if(strcmp(filtr,"s9")==0) splot(wsk,filtrlaps3);
              else {
              printf("Niepoprawny filtr!\n");
              return B_BRAKFILTRU; /*Jezeli nasz filtr jest niepoprawny zwroc blad*/
              }
        }

        if(nazwa_pliku_wy != NULL) zapisz(wybor->plik_wy,wsk);
        }
    free(wsk->piksele);  /*Zwalnianie pamieci*/
    wsk->piksele = NULL;
    free(wsk);
    wsk = NULL;

    return W_OK; /* ok: wej. strumien danych zainicjowany */
  } 
  else  return B_BRAKPLIKU;         /* blad:  nie otwarto pliku wejsciowego  */
  
}


