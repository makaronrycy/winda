# Projekt 4 - Winda
## Maurycy Szmuc 193194 i Mateusz Dublinowski 193182
Projekt polega na stworzeniu windy przemieszczającą ludzików pomiędzy 4 piętrami + parter. Zrealizowaliśmy go używając templatki win32app Visual Studio oraz GDI+.

- Do przemieszczania się winda używa uproszczonego algorytmu SCAN, który sortuje requesty w zależności kierunku podróży windy.

- Szybkość wykonywanej symulacji jest zależna od stałej interval. Aby uniknąć "migania" ekranu, używamy "double buffering". Polega to na zapisywaniu najpierw wszystkich rysunków które chcemy wykonać do bitmapy,
po czym bitmapa jest rysowana na ekranie.

- Winda sprawdza wagę pasażerów, i jeśli zostanie przekroczony limit zaprzestaje symulacji. Przy napotkaniu takiej sytaucji jest przycisk Reset, który zaczyna symulacje od nowa.

- Przy braku aktywności, winda zjeżdża na parter.
