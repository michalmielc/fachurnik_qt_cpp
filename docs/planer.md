# Plan projektu Fachurnik_C++

## Aktualny status

MENU
- [ ] osobna klasa
- [ ] poprawiæ opisy funkcjonalnoœci po prawej przy wyborze funkcji — `QPlainTextEdit`w trakcie
- [ ] dodaæ page porówywarka plików
- [ ] Podwójne Klikniêcie z menu przekierowuje na podstronê 1/5
- [ ] Podwójne Klikniêcie z menu przekierowuje na podstronê 2/5

PAGE FILE TO ESHOP 
- [ ] przycisk back to menu 
- [ ] tytu³ strony 
- [ ] Wczytanie nazwy pliku i œcie¿ki 
- [ ] Progress bar wczytywania
- [ ] Wczytywanie nag³ówka
- [ ] Walidacja pliku/ wielowalutowoœæ
- [ ] Zapis na pulpit jako DAT
- [ ] Zapis na pulpit jako CSV

AKTUALNIE WCZYTYWANIE PLIKU POPRAWNEGO I NIEPOPRAWNEGO INFO SCIEZKA I ROZSZERZENIE
BRAKUJE ILOSC LINII
- [ ] Osobna klasa `OpenFileDialog` dla wszystkich page — w trakcie
- [ ] Przycisk powrotu ze strony dla wszystkich page — w trakcie
- [ ] Wczytanie nazwy pliku i œcie¿ki — osobna klasa powi¹zana z `OpenFileDialog`
- [ ] Zapis do Excela
- [ ] Zmiana nag³ówka
- [ ] Zmiana waluty
- [ ] Zapis na pulpit jako CSV

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