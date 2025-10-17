# Wersja eskperymentalna!!!
## Po co?
Uznałem, że skoro mam zaimplementować hash table w c++ to zrobię coś podobnego, co twórca php Rasmus Lerdorf i zrobię gorszą wersję php.
## Co robi?
Można przypisywać wartości do zmiennich, tablic i je nadpisywać jak w pliku inputData.php.
## Jak odpalić?
Muszę się przyznać, że program kompiluje za mnie IDE, więc dobrym pomysłem jest uruchamianie go w IDE.
## Przykładowe dane wejściowe
```
<?php;
$liczba=10;
echo $liczba;

$liczba=15;
echo $liczba;

$nauczyciele=[
    "Szalkowski"=>"Maciej",
    "Kowalski"=>"Maciej",
    "Kamiński"=>"Maciej",
];

$nauczyciele["Szalkowski"]="Grzegorz";

echo $nauczyciele["Szalkowski"];
echo $nauczyciele["Kamiński"];

$nauczyciele["Switon"]="Dariusz";
echo $nauczyciele["Switon"];
```
Wynik:
```
10
15
Grzegorz
Maciej
Dariusz
```
## Złożoność algorytmiczna
### Klasa HashTable
| Metoda              | hashFunction(string s) | insert(const string& s, string key) | search(const string& s) | Ogólnie           |
|---------------------|------------------------|-------------------------------------|-------------------------|-------------------|
| Złożoność O czasowa | długość S              | długość S+kolizje                   | długość S+kolizje       | nie obowiązuje    |
| Złożoność miejscowa | 1                      | 1                                   | 1                       | Rozmiar tablicy N |
