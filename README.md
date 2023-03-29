# Przetwarzanie obrazów Lazik PPM i PGM
Prosta aplikacja stworzona w C i GraphicsMagick.

!(./images/zapis.ppm)

## Spis treści
* [Ogólne informacje](#ogólne-informacje)
* [Technologie](#technologie)
* [Ustawienia](#ustawienia)
* [Przykład uruchomienia](#przykład-uruchomienia)

## Ogólne informacje
Celem projektu jest zapoznanie się z dynamicznym alokowaniem pamięci w języku C oraz nauka przetwarzania obrazów PPM i PGM. Aplikacja służy do prostych zmian plików graficznych, wyświetlania ich i zapisywania do wybranego przez nas pliku.

## Technologie
* C99 +
* GraphicsMagick version 1.3.38

## Ustawianie
Aby uruchomić aplikację pierwszy raz, zainstaluj GraphicsMagick (instrukacja na stronie producenta), a następnie:

```
$ cd ../PrzetwObraz
$ make
```

## Opcje programu
Gdy program zostanie uruchomiony pierwszy raz można używać następujących opcji:

* -i [nazwa_pliku_do_odczytu] - otwieranie pliku na którym chcemy wykonywać operacje
* -o [nazwa_pliku_do_zapisu] - zapisanie do pliku naszych zmian na obrazie
* -p [wartość_progowania = 0.0 ... 1.0]- operacja półprogowania bieli dla wybranych wartości
* -n - operacja negatywu
* -k - opearcja konturowania
* -s [wartosc_filtra = 's1' ... 's9'] - operacja nakładająca filtr na obraz
* -d - wyświetlanie aktualnego wybranego obrazu w aplikacji

## Przykład uruchomienia

### Wyświetlanie obrazu
```
$ ./PrzetwObraz -i src_images/kubus.ppm -d
```
![Program przykład dzialania](./images/oryginalny_kubus.png)

### Wykonywanie operacji na obrazie i zapis pracy
```
$ ./PrzetwObraz -i src_images/kubus.ppm -p 0.5 -k -o zapis
```
### Wyświetlanie wyniku
```
$ ./PrzetwObraz -i zapis -d
```
![Program przykład wyswietlania](./images/Zmieniony_kubus.png)
## Źródła
Projekt powstał na potrzeby zajęć u Dr. inż. Roberta Muszyńskiego przy pomocy części kodu udostępnionego dla studentów (plik  ../src/opcje.c).