# Plan projektu Fachurnik_C++

## Aktualny status
#####################################################################################################
MENU
- [x] osobna klasa
- [ ] poprawi� opisy funkcjonalno�ci po prawej przy wyborze funkcji � `QPlainTextEdit`w trakcie
- [ ] doda� page por�wywarka plik�w
- [x] Podw�jne Klikni�cie z menu przekierowuje na podstron� 1/5 eshop file to eshop
- [x+ ] Podw�jne Klikni�cie z menu przekierowuje na podstron� 2/5 clone eshop files
#####################################################################################################
PAGE FILE TO ESHOP
- [ ] CONTROLS  
- [x] Btn back to menu 
- [x] OPEN FILE menu 
- [x] Lbl ścieżka pliku IN
- [x] Lbl nazwa pliku IN
- [x] Lbl liczba linii IN
- [x] RadioBtn kan dystrybucji IN/OUT
- [x] LEdit nr klienta IN/OUT
- [x] LEdit data od do  IN/OUT
- [x] LEdit nr katalogu IN/OUT
- [x] LEdit waluta IN/OUT
- [x] LEdit gr rabatowa IN/OUT
- [x] LEdit sales rep IN/OUT
- [x] ChBox dodatek stop IN/OUT
- [x] ChBox oferta specjalna IN/OUT
- [ ] ChBox katalog niemiecki( nie działa)
- [x] RadioBtn waluta EUR i PLN IN/OUT
- [x] LEdit kurs IN/OUT
- [x] Btn Export csv OUT
- [x] Btn Export dat OUT

- [x] Wczytanie nazwy pliku i �cie�ki 
- [x ] Progress bar wczytywania
- [x] Wczytywanie nag��wka
- [x] Konstrukcja nag��wka
- [x] Walidacja pliku/ wielowalutowo��
- [x] Zapis na pulpit jako DAT
- [x] Zapis na pulpit jako CSV

----------------------------------------------
TESTY ESHOP

----------------------------------------------
#####################################################################################################

PAGE CLONE FILES TO ESHOP

- [ ] CONTROLS  
- [x] Btn back to menu 
- [x] OPEN DAT FILE menu 
- [x] Lbl ścieżka pliku IN
- [x] Lbl liczba linii IN
- [x] Lbl nazwa pliku IN
- [x] OPEN csv FILE menu 
- [x] Lbl ścieżka pliku IN
- [x] Lbl liczba linii IN
- [x] Lbl nazwa pliku IN
- [x] RadioBtn kan dystrybucji IN/OUT
- [x] LEdit nr klienta IN/OUT
- [x] LEdit data od do  IN/OUT
- [x] LEdit nr katalogu IN/OUT
- [x] LEdit waluta IN/OUT
- [x] LEdit gr rabatowa IN/OUT
- [x] LEdit sales rep IN/OUT
- [x] ChBox dodatek stop IN/OUT
- [x] ChBox oferta specjalna IN/OUT
- [ ] ChBox katalog niemiecki( nie działa)
- [x] Btn CLONE OUT

- [x] Wczytanie nazwy pliku DAT i �cie�ki 
- [x] Progress bar wczytywania DAT
- [x] Wczytanie nazwy pliku CSV i �cie�ki 
- [x] Progress bar wczytywania CSV
- [x] Wczytywanie nag��wka
- [x] Konstrukcja nag��wka
- [x] Walidacja pliku/ wielowalutowo��
- [x] Zapis na pulpit jako DAT
- [x] Zapis na pulpit jako CSV

----------------------------------------------
TESTY ESHOP

----------------------------------------------
#####################################################################################################
Page3 CSV to eshop
PAGE CLONE FILES TO ESHOP

- [ ] CONTROLS  
- [x] Btn back to menu 
- [x] OPEN DAT FILE menu 
- [x] Lbl ścieżka pliku IN
- [x] Lbl liczba linii IN
- [x] Lbl nazwa pliku IN
- [x] OPEN csv FILE menu 
- [x] Lbl ścieżka pliku IN
- [x] Lbl liczba linii IN
- [x] Lbl nazwa pliku IN
- [x] RadioBtn kan dystrybucji IN/OUT
- [x] LEdit nr klienta IN/OUT
- [x] LEdit data od do  IN/OUT
- [x] LEdit nr katalogu IN/OUT
- [x] LEdit waluta IN/OUT
- [x] LEdit gr rabatowa IN/OUT
- [x] LEdit sales rep IN/OUT
- [x] ChBox dodatek stop IN/OUT
- [x] ChBox oferta specjalna IN/OUT
- [ ] ChBox katalog niemiecki( nie działa)
- [x] Btn CLONE OUT

- [x] Wczytanie nazwy pliku DAT i �cie�ki 
- [x] Progress bar wczytywania DAT
- [x] Wczytanie nazwy pliku CSV i �cie�ki 
- [x] Progress bar wczytywania CSV
- [x] Wczytywanie nag��wka
- [x] Konstrukcja nag��wka
- [x] Walidacja pliku/ wielowalutowo��
- [x] Zapis na pulpit jako DAT
- [x] Zapis na pulpit jako CSV

TESTY ESHOP

#####################################################################################################

PAGE 4 DAT FILE TO BMECAT

- [ ] CONTROLS  
- [x] Btn back to menu 
- [x] OPEN FILE menu 
- [x] Lbl ścieżka pliku IN
- [x] Lbl nazwa pliku IN
- [x] Lbl liczba linii IN
- [x] RadioBtn kan dystrybucji IN/OUT
- [x] LEdit nr klienta IN/OUT
- [x] LEdit data od do  IN/OUT
- [x] LEdit nr katalogu IN/OUT
- [x] LEdit waluta IN/OUT
- [x] LEdit gr rabatowa IN/OUT
- [x] LEdit sales rep IN/OUT
- [x] ChBox dodatek stop IN/OUT
- [x] ChBox oferta specjalna IN/OUT
- [x] ChBox katalog niemiecki( nie działa)
- [x] RadioBtn waluta EUR i PLN IN/OUT
- [x] LEdit kurs IN/OUT
- [x] Btn Export csv OUT

- [x] Wczytanie nazwy pliku i �cie�ki 
- [x] Progress bar wczytywania
- [x] Wczytywanie nag��wka
- [x] Konstrukcja nag��wka
- [x] Walidacja pliku/ wielowalutowo��
- [x] Zapis na pulpit jako CSV

----------------------------------------------

TESTY

----------------------------------------------


#####################################################################################################

PAGE 5 create CIF FILE 

- [ ] CONTROLS  
- [x] Btn back to menu 
- [ ] OPEN CIV FILE  

#####################################################################################################


## Architektura

### UI

- `QStackedWidget` � prze��czanie stron
- `QTreeWidget` � menu po lewej stronie
- `QPlainTextEdit` � opis funkcjonalno�ci po prawej stronie

### Klasy pomocnicze

- `MenuTreeBuilder` � budowanie menu
- `OpenFileDialog` � wyb�r pliku
- `BtnBackToMenu` � powr�t do strony g��wnej
- `FileLoader` � planowana klasa do wczytywania pliku
- FileLoadingProgress.h - planowane okno post�pu wczytwyanie pliku



## Nast�pne kroki

1. Doko�czy� `BtnBackToMenu`
2. Doda� klas� `FileLoader`
3. Wy�wietli� nazw� pliku i �cie�k� w UI
4. Przygotowa� zapis do Excela
5. Doda� obs�ug� zmiany nag��wka
6. Doda� obs�ug� zmiany waluty