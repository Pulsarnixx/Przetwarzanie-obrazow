#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../inc/obs.h"
#include "../inc/fun.h"
#include "../inc/opcje.h"

int czyZgodnie = 0; 
/*UWAGA: Kolejnosc wykonywania operacji na obrazach w poniższym programie jest scisle okreslona */
/*przy wywolywaniu wszystkich funkcji operacje beda wykonywane nastepujaco:*/
/*
1. negatyw
2. konturowanie
3. polprogowanie bieli
4. splot z wykorzystaniem odpowiedniego filtru
*/
int main(int argc, char ** argv) {
  
 
  t_opcje opcje;
  int kod_bledu;
  
  kod_bledu = przetwarzaj_opcje(argc,argv,&opcje); /*Ustala czy wystapil blad*/
  if(czyZgodnie == 1) kod_bledu = B_NIEPOPRAWNYPLIK;
  if (kod_bledu)
    printf("Blad nr %d\n", kod_bledu);
  else
    printf("Opcje poprawne\n");

  return kod_bledu;
}



