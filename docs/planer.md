# Plan projektu Fachurnik_C++

## Aktualny status

- [ ] MENU
- [ ] opis funkcjonalnoœci po prawej przy wyborze funkcji — `QPlainTextEdit`w trakcie
- [ ] Podwójne Klikniêcie z menu przekierowuje na podstronê 1/5
- [ ] PAGE ESHOPTO ESHOP 
- [x] przycisk back to menu 
- [x] Wczytanie nazwy pliku i œcie¿ki 
- [x] Progress bar wczytywania


AKTUALNIE WCZYTYWANIE PLIKU POPRAWNEGO I NIEPOPRAWNEGO INFO SCIEZKA I ROZSZERZENIE
BRAKUJE ILOSC LINII
- [ ] Osobna klasa `OpenFileDialog` dla wszystkich page — w trakcie
- [ ] Przycisk powrotu ze strony dla wszystkich page — w trakcie
- [ ] Wczytanie nazwy pliku i œcie¿ki — osobna klasa powi¹zana z `OpenFileDialog`
- [ ] Zapis do Excela
- [ ] Zmiana nag³ówka
- [ ] Zmiana waluty

## Architektura

### UI

- `QStackedWidget` — prze³¹czanie stron
- `QTreeWidget` — menu po lewej stronie
- `QPlainTextEdit` — opis funkcjonalnoœci po prawej stronie

### Klasy pomocnicze

- `MenuTreeBuilder` — budowanie menu
- `OpenFileDialog` — wybór pliku
- `BtnBackToMenu` — powrót do strony g³ównej
- `FileLoader` — planowana klasa do wczytywania pliku
- FileLoadingProgress.h - planowane okno postêpu wczytwyanie pliku



## Nastêpne kroki

1. Dokoñczyæ `BtnBackToMenu`
2. Dodaæ klasê `FileLoader`
3. Wyœwietliæ nazwê pliku i œcie¿kê w UI
4. Przygotowaæ zapis do Excela
5. Dodaæ obs³ugê zmiany nag³ówka
6. Dodaæ obs³ugê zmiany waluty